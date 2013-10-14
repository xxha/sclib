/////////jpeg2LCD.c

#include "framebuffer.h"
#include "jpeglib.h"

#include <setjmp.h>

// error handler, to avoid those pesky exit(0)'s

struct my_error_mgr {
	struct jpeg_error_mgr pub; /* "public" fields */
	jmp_buf setjmp_buffer; /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


METHODDEF(void) my_error_exit (j_common_ptr cinfo);

//
// to handle fatal errors.
// the original JPEG code will just exit(0). can't really
// do that in Windows....
//

METHODDEF(void) my_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr) cinfo->err;

	char buffer[JMSG_LENGTH_MAX];

	/* Create the message */
	(*cinfo->err->format_message) (cinfo, buffer);

	/* Always display the message. */
	//MessageBox(NULL,buffer,"JPEG Fatal Error",MB_ICONSTOP);


	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

int Jpeg2LCD(char * filename, int x,int y)
{
	// get our buffer set to hold data
	unsigned short *p = (unsigned short *)fbdev.fb_mem;//定义framebuffer大的内存空间
	int width=0;
	int height=0;

	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	FILE * infile=NULL;	/* source file */

	JSAMPARRAY buffer;	/* Output row buffer */
	int row_stride;	/* physical row width in output buffer */

	if ((infile = fopen(filename, "rb")) == NULL) {
		printf("JPEG :\nCan't open %s\n", filename);
		return -1;
	}

	/* Step 1: allocate and initialize JPEG decompression object */

	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;


	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		if (infile!=NULL)
			fclose(infile);
		return -1;
	}

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);

	/* Step 2: specify data source (eg, a file) */

	jpeg_stdio_src(&cinfo, infile);

	/* Step 3: read file parameters with jpeg_read_header() */

	(void) jpeg_read_header(&cinfo, TRUE);

	/* Step 4: set parameters for decompression */

/* In this example, we don't need to change any of the defaults set by
* jpeg_read_header(), so we do nothing here.
*/

	/* Step 5: Start decompressor */

	(void) jpeg_start_decompress(&cinfo);


	// how big is this thing gonna be?
	width = cinfo.output_width;
	height = cinfo.output_height;

	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;

	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);


/* Here we use the library's state variable cinfo.output_scanline as the
* loop counter, so that we don't have to keep track ourselves.
*/
	p+=(y)*fbdev.fb_line_len/2;//文件指针指向开始
	while (cinfo.output_scanline < cinfo.output_height) {
		int count;
		unsigned char * buf;

		(void) jpeg_read_scanlines(&cinfo, buffer, 1);

		buf=buffer[0];
		if (cinfo.out_color_components==3) {
			for (count=0;count<width;count++) {
				//buffer[count*3]; R
				//buffer[count*3+1]; G
				//buffer[count*3+2]; B
				p[x+count]=((buf[count*3]>>3)<<11)|((buf[count*3+1]>>2)<<5)|(buf[count*3+2]>>3);
			}
		} else if (cinfo.out_color_components==1) {
			for (count=0;count<width;count++) {
				p[x+count]=((buf[count]>>3)<<11)|((buf[count]>>2)<<5)|(buf[count]>>3);
			}
		}
		p+=fbdev.fb_line_len/2;
	}

	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);
	return 0;
}

int main (void)
{
	framebuffer_open();

	Jpeg2LCD("image.jpg",0,0);
	usleep(1000*100);

	framebuffer_close();
	return 0;
}
