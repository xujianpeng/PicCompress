#include "Image.h"
#include "jpeg/jpeglib.h"
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */
 
  jmp_buf setjmp_buffer;	/* for return to caller */
};
 
typedef struct my_error_mgr * my_error_ptr;
 
//Here's the routine that will replace the standard error_exit method:
 
METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
 
  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);
 
  /* Return control to the setjmp point */
 longjmp(myerr->setjmp_buffer, 1);
}

void writeJpg(const char *fileName, CommPicInfo *picInfo)
{
    if(picInfo == NULL || picInfo->data == NULL) 
    {   
        printf("CommPicInfo picInfo or picInfo->data NULL error!\n");
        return ;
    }
    unsigned char *dataPtr = picInfo->data;

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);
 
	if ((outfile = fopen(fileName, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\n", fileName);
		return ;
	}
	jpeg_stdio_dest(&cinfo, outfile);
 
	cinfo.image_width = picInfo->width; 	/* image width and height, in pixels */
	cinfo.image_height = picInfo->height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
 
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, picInfo->quality, TRUE /* limit to baseline-JPEG values */);
 
    cinfo.dct_method = JDCT_IFAST;
    cinfo.scale_num = 1;
    cinfo.scale_denom = picInfo->scale_denom;

	jpeg_start_compress(&cinfo, TRUE);
 
	row_stride = picInfo->width * 3;	/* JSAMPLEs per row in image_buffer */
 
	// printf("width %d height %d\n", picInfo->width, picInfo->height);	

	while (cinfo.next_scanline < cinfo.image_height) 
    {
		row_pointer[0] = &dataPtr[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
 
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
}
