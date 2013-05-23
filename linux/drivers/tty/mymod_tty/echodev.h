#ifndef __ECHODEV_H__
#define __ECHODEV_H__

#include <linux/workqueue.h>
#include <asm/atomic.h>

#define ECHODEV_BUFSIZE 32

struct echodev {
	char buffer[ECHODEV_BUFSIZE];
	atomic_t nbuf;
	void (*rx_fn)(void *, const unsigned char *, int);
	int r, w;
	struct work_struct work;
	void *priv;
};

struct echodev *echodev_init (void *, void (*)(void*, const unsigned char*, int));
void echodev_destroy (struct echodev *);
int echodev_write(struct echodev *, const unsigned char *, int);
int echodev_free(struct echodev *);

#endif
