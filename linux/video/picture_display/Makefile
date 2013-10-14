all: bmp_display jpeg_display fbtools practice1 practice2 jpeg2LCD

bmp_display: bmp_display.c
	gcc bmp_display.c -o bmp_display

jpeg_display: jpeg_display.c
	gcc jpeg_display.c -o jpeg_display -ljpeg

jpeg2LCD: jpeg2LCD.c framebuffer.h
	gcc jpeg2LCD.c framebuffer.h -o jpeg2LCD -l jpeg

fbtools: fbtools.h fbtools.c
	gcc fbtools.c fbtools.h -o fbtools

practice1: practice1.c
	gcc practice1.c -o practice1

practice2: practice2.c
	gcc practice2.c -o practice2

clean:
	rm -rf *.o bmp_display jpeg_display fbtools practice1 practice2 jpeg2LCD
