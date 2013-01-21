#ifndef __CORE_H__
#define __CORE_H__

struct image_info {
	unsigned char *raw;
	unsigned char *raw1; /* modified raw */
	int color_space; /* JCS_RGB: 2 or JCS_GRAYSCALE: 1 */
	int width;
	int height;
	int bytes_per_pixel;
};

void image_clean(struct image_info *iif);
void image_swap(struct image_info *iif);
#endif
