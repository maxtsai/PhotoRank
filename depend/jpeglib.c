#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "jpeg.h"


int read_jpeg(char *filename, struct image_info *iif, int desired_color_space)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned char *raw_image = NULL;
	int ret = 0;

	/* libjpeg data structure for storing one row, that is, scanline of an image */
	JSAMPROW row_buf[1];

	if ((!iif) || (!filename))
		return -1;
	if ((desired_color_space < 1) || (desired_color_space > 2))
		return -1;

	memset(iif, 0, sizeof(struct image_info));

	FILE *infile = fopen( filename, "rb" );
	unsigned long location = 0;
	int i = 0, j = 0;

	if ( !infile )
	{
		printf("open %s fails (%s)\n!", filename, strerror(errno));
		return -1;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	/* this makes the library read from infile */
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	raw_image = (unsigned char*) malloc(cinfo.output_width *
			cinfo.output_height *
			cinfo.num_components);

	row_buf[0] = (unsigned char *) malloc(cinfo.output_width *
			cinfo.num_components );

	while( cinfo.output_scanline < cinfo.image_height )  {
		jpeg_read_scanlines(&cinfo, row_buf, 1);
		for( i = 0; i < cinfo.image_width * cinfo.num_components; i++)
			raw_image[location++] = row_buf[0][i];
	}

	iif->color_space = desired_color_space;
	iif->width = cinfo.image_width;
	iif->height = cinfo.image_height;
	iif->bytes_per_pixel = cinfo.num_components;
	iif->raw = NULL;
	iif->m_raw = (unsigned char *) malloc(cinfo.output_width *
			cinfo.num_components );
	memset(iif->m_raw, 0, cinfo.output_width * cinfo.num_components);

	if (desired_color_space == JCS_RGB) {
		iif->raw = raw_image;
	} else if (desired_color_space == JCS_GRAYSCALE) {
		iif->bytes_per_pixel = 1; /* grayscale */
		iif->raw = (unsigned char*) malloc(cinfo.output_width *
				cinfo.output_height);
		int total_size = cinfo.output_width * cinfo.output_height *
			cinfo.num_components;
		for (i = j = 0; i < total_size; i += cinfo.num_components, j++)
#if 0
			iif->raw[j] = (unsigned char)
				(0.3 * raw_image[i] + 0.59 * raw_image[i+1] + 0.11 * raw_image[i+2]);
#else
			iif->raw[j] = (unsigned char) raw_image[i+1];
#endif
		free(raw_image);
	} else {
		printf("unable to meet desired color space (image: %d, desired: %d)\n",
				cinfo.jpeg_color_space,
				desired_color_space);
		ret = -1;
		memset(iif, 0, sizeof(struct image_info));
		free(raw_image);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(row_buf[0]);
	fclose(infile);

	return ret;
}

int write_jpeg(char *filename, struct image_info *iif)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	if (!filename || !iif || !(iif->raw))
		return -1;

	JSAMPROW row_buf[1];
	FILE *outfile = fopen(filename, "wb");

	if ( !outfile )
	{
		printf("Error opening output jpeg file %s\n!", filename);
		return -1;
	}
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = iif->width;
	cinfo.image_height = iif->height;
	cinfo.input_components = iif->bytes_per_pixel;
	cinfo.in_color_space = iif->color_space;

	jpeg_set_defaults(&cinfo);
	jpeg_start_compress(&cinfo, TRUE);
	while(cinfo.next_scanline < cinfo.image_height)
	{
		row_buf[0] = &iif->raw[cinfo.next_scanline *
			cinfo.image_width *
			cinfo.input_components];
		jpeg_write_scanlines(&cinfo, row_buf, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);
	return 0;
}






