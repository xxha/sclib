/*
   BeatForce
   ringbuffer.c  - ring buffer's
   
   Copyright (c) 2001, Patrick Prasse (patrick.prasse@gmx.net)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public Licensse as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* This source has been modified from its original form by David LeBlanc
   (leblancdw@yahoo.com) for the following reasons:
   - Converted to use Linux kernel APIs and headers
   - Added error checking if buffer allocation fails
   - A function called rb_destroy() has also been provided to free a ring
     buffer structure previously allocated with rb_init()
   - Introduced a data_size member to fix a corner case or two
 */

#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/random.h>
#include <linux/moduleparam.h>
#include "ringbuffer.h"

static int preempt = 0;
module_param(preempt, bool, 0);

static void *
my_memcpy(void *dest, const void *src, size_t count)
{
        char *tmp = dest;
        const char *s = src;

        while (count--) {
                *tmp++ = *s++;
		if (preempt)
		    schedule_timeout_uninterruptible(random32() & 3);
	}
        return dest;
}

/* call before output thread is active !!!!! */
int
rb_init (struct OutRingBuffer **rb, int size)
{
    struct OutRingBuffer *ring;

    if(rb==NULL || size < 1024)
    {
        return -EINVAL;
    }
    
    ring = kzalloc (sizeof (struct OutRingBuffer), GFP_KERNEL);
    if(ring == NULL)
    {
        return -ENOMEM;
    }

    ring->size = 1;
    while(ring->size < size)
        ring->size <<= 1;


    ring->buffer=kmalloc(sizeof(char)*(ring->size), GFP_KERNEL);
 
    if(ring->buffer == NULL)
    {
        kfree(ring);
        return -ENOMEM;
    }
   
    *rb = ring;

    return 0;
}

void
rb_destroy (struct OutRingBuffer *rb)
{
    kfree(rb->buffer);
    kfree(rb);
}

int
rb_write (struct OutRingBuffer *rb, unsigned char * buf, int len)
{
    int total;
    int i;

    /* total = len = min(space, len) */
    total = rb_free(rb);
    if(len > total)
        len = total;
    else
        total = len;

    i = rb->wr_pointer;
    if(i + len > rb->size)
    {
        my_memcpy(rb->buffer + i, buf, rb->size - i);
        buf += rb->size - i;
        len -= rb->size - i;
        rb->data_size += rb->size - i;
        i = 0;
    }
    my_memcpy(rb->buffer + i, buf, len);
    rb->wr_pointer = i + len;
    rb->data_size += len;
    return total;

        
}

int
rb_free (struct OutRingBuffer *rb)
{
    return (rb->size - rb->data_size);
}


int
rb_read (struct OutRingBuffer *rb, unsigned char * buf, int max)
{
    int total;
    int i;
    /* total = len = min(used, len) */
    total = rb_data_size(rb);

    if(max > total)
        max = total;
    else
        total = max;

    i = rb->rd_pointer;
    if(i + max > rb->size)
    {
        my_memcpy(buf, rb->buffer + i, rb->size - i);
        buf += rb->size - i;
        max -= rb->size - i;
        rb->data_size -= rb->size - i;
        i = 0;
    }
    my_memcpy(buf, rb->buffer + i, max);
    rb->rd_pointer = i + max;
    rb->data_size -= max;
    return total;

}

int
rb_data_size (struct OutRingBuffer *rb)
{
    return rb->data_size;
}


int
rb_clear (struct OutRingBuffer *rb)
{
    memset(rb->buffer,0,rb->size);
    rb->rd_pointer=0;
    rb->wr_pointer=0;
    rb->data_size=0;
    return 0;
}





