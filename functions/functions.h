#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

int sobel(struct image_info *iif, double threshold);
void canny_edge_detect(struct image_info *iif);
void gaussian_noise_reduce(struct image_info *iif);

float calc_blur(struct image_info *iif);
#endif
