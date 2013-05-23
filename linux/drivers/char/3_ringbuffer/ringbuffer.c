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
#include "ringbuffer.h"


/* call before output thread is active !!!!! */
int rb_init (struct OutRingBuffer **rb, int size)
{
	struct OutRingBuffer *ring;

	if(rb==NULL || size < 1024)
	{
		return -EINVAL;
	}
	//alloc and memset OutRingBuffer
	ring = kzalloc (sizeof (struct OutRingBuffer), GFP_KERNEL);
	if(ring == NULL)
	{
		return -ENOMEM;
	}

	ring->size = 1;
	while(ring->size < size)
		ring->size <<= 1;

	//alloc ring->buffer, size = ring->size
	ring->buffer=kmalloc(sizeof(char)*(ring->size), GFP_KERNEL);
 
	if(ring->buffer == NULL)
	{
		kfree(ring);
		return -ENOMEM;
	}
   
	*rb = ring;

	return 0;
}

void rb_destroy (struct OutRingBuffer *rb)
{
	kfree(rb->buffer);
	kfree(rb);
}

/*
 * len --- the size you want to write into RB
 * buf --- the buffer you want to write into RB
 * rb --- the OutRingBuffer you want to write into
*/
int rb_write (struct OutRingBuffer *rb, unsigned char * buf, int len)
{
	int total;
	int i;

	//total = rb space left
	total = rb_free(rb);

	/* total = len =min(space, len) */
	if(len > total)
		len = total;
	else
		total = len;

	i = rb->wr_pointer;

        //如果数据长度超过了rb->size
	if(i + len > rb->size)
	{
                //从rb->wr_pointer 位置开始写入新数据，写入大小是rb剩下的部分
		memcpy(rb->buffer + i, buf, rb->size - i);
                //计算新buf地址，以后写入有用
		buf += rb->size - i;
                //计算需要写入的剩余长度
		len -= rb->size - i; 
                //计算写入后新的buffer大小
		rb->data_size += rb->size - i;
		i = 0;
	}
        //环状buffer就是这里实现的，这里从头开始写入数据，实现回环。
	memcpy(rb->buffer + i, buf, len);
        //写入新的wr_p
	rb->wr_pointer = i + len;
        //写入新的buffer size
	rb->data_size += len;
	return total;
}

int rb_free (struct OutRingBuffer *rb)
{
	return (rb->size - rb->data_size);
}


int
rb_read (struct OutRingBuffer *rb, unsigned char * buf, int max)
{
	int total;
	int i;
	/* total = max = min(used, len) */
	total = rb_data_size(rb);

	if(max > total)
		max = total;
	else
		total = max;

	i = rb->rd_pointer;
	if(i + max > rb->size)
	{
                //先把剩余的rb->buffer 读完
		memcpy(buf, rb->buffer + i, rb->size - i);
		buf += rb->size - i;
		max -= rb->size - i;
		rb->data_size -= rb->size - i;
		i = 0;
	}

        //这里实现环状读写，读完了一个rb->buffer之后，又从头开始读入数据，从而实现回环。
	memcpy(buf, rb->buffer + i, max);
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





