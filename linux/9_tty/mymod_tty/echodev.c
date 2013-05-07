#include <linux/slab.h>
#include "echodev.h"

static void
__echodev_work_fn(struct work_struct *work)
{
	struct echodev *e = container_of(work, struct echodev, work);
	int nbytes;

	if ((nbytes = atomic_read(&e->nbuf))) {
		if (e->r + nbytes > ECHODEV_BUFSIZE) {
			int i = ECHODEV_BUFSIZE - e->r;
			e->rx_fn(e->priv, e->buffer + e->r, i);
			e->r = nbytes - i;
			e->rx_fn(e->priv, e->buffer, e->r);
		} else {
			e->rx_fn(e->priv, e->buffer + e->r, nbytes);
			e->r += nbytes;
		}

		atomic_sub(nbytes, &e->nbuf);
	}
}

struct echodev*
echodev_init(void *priv, void (*rx_fn)(void*, const unsigned char*, int))
{
	struct echodev *e = kzalloc(sizeof *e, GFP_KERNEL);

	if(!e)
		return NULL;

	e->priv = priv;
	e->rx_fn = rx_fn;
	INIT_WORK(&e->work, __echodev_work_fn);

	return e;
}

void
echodev_destroy(struct echodev *e)
{
	cancel_work_sync(&e->work);
	kfree(e);
}

int
echodev_write (struct echodev *e, const unsigned char * buf, int nbytes)
{
	int nfree = ECHODEV_BUFSIZE - atomic_read(&e->nbuf);

	if (nbytes > nfree)
		nbytes = nfree;

	if (e->w + nbytes > ECHODEV_BUFSIZE) {
		int i = ECHODEV_BUFSIZE - e->w;
		memcpy(e->buffer + e->w, buf, i);
		e->w = nbytes - i;
		memcpy(e->buffer, buf + i, e->w);
	} else {
		memcpy(e->buffer + e->w, buf, nbytes);
		e->w += nbytes;
	}

	atomic_add(nbytes, &e->nbuf);
	schedule_work(&e->work);

	return nbytes;
}

int
echodev_free(struct echodev *e)
{
	return ECHODEV_BUFSIZE - atomic_read(&e->nbuf);
}
