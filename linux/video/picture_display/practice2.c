/*
 * draw a single point on screen.
 * locate at (x,y) = (100, 100)
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

int main ()
{
	int fp=0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long screensize=0;
	char *fbp = 0;
	int x = 0, y = 0;
	long location = 0;

	fp = open ("/dev/fb0",O_RDWR);
	if (fp < 0) {
		printf("Error : Can not open framebuffer device/n");
		exit(1);
	}

	if (ioctl(fp,FBIOGET_FSCREENINFO,&finfo)) {
		printf("Error reading fixed information/n");
		exit(2);
	}

	if (ioctl(fp,FBIOGET_VSCREENINFO,&vinfo)) {
		printf("Error reading variable information/n");
		exit(3);
	}

	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	printf("screensize = %ld\n", screensize);

	fbp =(char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp,0);

	if ((int) fbp == -1) {
		printf ("Error: failed to map framebuffer device to memory./n");
		exit (4);
	}

	x = 100;
	y = 100;
	location = x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;

	*(fbp + location) = 100;
	*(fbp + location + 1) = 15;
	*(fbp + location + 2) = 200;
	*(fbp + location + 3) = 0;

	munmap (fbp, screensize);
	close (fp);
	return 0;
}
