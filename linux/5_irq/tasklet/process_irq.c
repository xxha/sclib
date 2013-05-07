#include <linux/delay.h>
#include <linux/random.h>

/* introduce artificial delay to simulate real work */
void process_irq(void)
{
	udelay(random32() & 0xffff );
}

