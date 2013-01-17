#ifndef __JPEGLIB_H__
#define __JPEGLIB_H__

struct image_info {
	unsigned char *raw;
	int color_space; /* JCS_RGB: 2 or JCS_GRAYSCALE: 1 */
	int width;
	int height;
	int bytes_per_pixel;
};

int read_jpeg(char *filename, struct image_info *iif, int desired_color_space);

#endif
