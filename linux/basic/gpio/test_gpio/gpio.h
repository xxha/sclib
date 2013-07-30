/* By Yanjun Luo, for MX27 gpio test with application software. */

#ifndef __MX27_GPIO_H__
#define __MX27_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>

#define	PORT_A						0x0
#define	PORT_B						0x1
#define	PORT_C						0x2
#define	PORT_D						0x3
#define	PORT_E						0x4
#define	PORT_F						0x5

#define	OUTPUT						0x1
#define INPUT						0x0

extern int setgpiodir(unsigned int port, unsigned int pin, unsigned int dir);
extern int setgpiodata(unsigned int port, unsigned int pin, unsigned int data);
extern int getgpiodata(unsigned int port, int pin, unsigned int * data);
extern int setpullup(unsigned int port, int pin, unsigned int pull);
extern int gpio_init();
extern int gpio_exit();


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __MX27_GPIO_H__ */
