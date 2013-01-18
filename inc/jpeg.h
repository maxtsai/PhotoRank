#ifndef __JPEG_H__
#define __JPEG_H__

struct image_info {
	unsigned char *raw;
	unsigned char *m_raw; /* modified raw */
	int color_space; /* JCS_RGB: 2 or JCS_GRAYSCALE: 1 */
	int width;
	int height;
	int bytes_per_pixel;
};

int read_jpeg(char *filename, struct image_info *iif, int desired_color_space);
int write_jpeg(char *filename, struct image_info *iif);

#endif
