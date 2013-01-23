/*
	FAST-EDGE
	Copyright (c) 2009 Benjamin C. Haynor

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "core.h"
#include "fast-canny.h"

void canny_edge_detect(struct image_info *iif) {
	struct image_info img_scratch;
	int high, low;

	if (iif->bytes_per_pixel != 1) {
		printf("Only accept grayscale so far.\n");
		return;
	}

	int *g = malloc(iif->width * iif->height * sizeof(int));
	int *dir = malloc(iif->width * iif->height * sizeof(int));
	unsigned char *img_scratch_data = malloc(iif->width *iif->height
			* sizeof(char));

	memset(iif->raw1, 0, iif->width * iif->height * iif->bytes_per_pixel);

	img_scratch.width = iif->width;
	img_scratch.height = iif->height;
	img_scratch.raw = img_scratch_data;
	img_scratch.raw1 = NULL;

	calc_gradient_sobel(iif, g, dir);
	non_max_suppression(&img_scratch, g, dir);
	estimate_threshold(&img_scratch, &high, &low);
	hysteresis(high, low, &img_scratch, iif);

	free(g);
	free(dir);
	free(img_scratch_data);
}

void hysteresis(int high, int low, struct image_info * img_in, struct image_info * img_out)
{
	int x, y, n, max;
	max = img_in->width * img_in->height;
	for (n = 0; n < max; n++)
		img_out->raw1[n] = 0xFF;
	for (y=0; y < img_out->height; y++) {
		for (x=0; x < img_out->width; x++) {
			if (img_in->raw[y * img_out->width + x] >= high) {
				trace (x, y, low, img_in, img_out);
			}
		}
	}
}

int trace(int x, int y, int low, struct image_info * img_in, struct image_info * img_out)
{
	int y_off, x_off;

	if (img_out->raw1[y * img_out->width + x] == 0xFF)
	{
		img_out->raw1[y * img_out->width + x] = 0x00;
		for (y_off = -1; y_off <=1; y_off++)
		{
			for(x_off = -1; x_off <= 1; x_off++)
			{
				if (!(y == 0 && x_off == 0) &&
						range(img_in, x + x_off, y + y_off) &&
						img_in->raw[(y + y_off) * img_out->width + x + x_off] >= low) {
					if (trace(x + x_off, y + y_off, low, img_in, img_out))
					{
					    return(1);
					}
				}
			}
		}
		return(1);
	}
	return(0);
}

int range(struct image_info * img, int x, int y)
{
	if ((x < 0) || (x >= img->width)) {
		return(0);
	}
	if ((y < 0) || (y >= img->height)) {
		return(0);
	}
	return(1);
}

void estimate_threshold(struct image_info *iif, int *high, int *low) {
	int i, max, pixels, high_cutoff;
	int histogram[256];

	max = iif->width * iif->height;

	for (i = 0; i < 256; i++)
		histogram[i] = 0;

	for (i = 0; i < max; i++)
		histogram[iif->raw[i]]++;

	pixels = (max - histogram[0]) * HIGH_THRESHOLD_PERCENTAGE;
	high_cutoff = 0;
	i = 255;
	while (high_cutoff < pixels) {
		high_cutoff += histogram[i];
		i--;
	}
	*high = i;

	i = 1;
	while (histogram[i] == 0) {
		i++;
	}
	*low = (*high + i) * LOW_THRESHOLD_PERCENTAGE;

	#ifdef PRINT_HISTOGRAM
	for (i = 0; i < 256; i++) {
		printf("i %d count %d\n", i, histogram[i]);
	}
	#endif
}

void non_max_suppression(struct image_info *iif, int g[], int dir[]) {
	int w, h, x, y, max_x, max_y;
	w = iif->width;
	h = iif->height;
	max_x = w;
	max_y = w * h;
	for (y = 0; y < max_y; y += w) {
		for (x = 0; x < max_x; x++) {
			switch (dir[x + y]) {
				case 0:
					if (g[x + y] > g[x + y - w] && g[x + y] > g[x + y + w]) {
						if (g[x + y] > 255)
							iif->raw[x + y] = 0xFF;
						else
							iif->raw[x + y] = g[x + y];
					} else
						iif->raw[x + y] = 0x00;
					break;
				case 1:
					if (g[x + y] > g[x + y - w - 1] && g[x + y] > g[x + y + w + 1]) {
						if (g[x + y] > 255)
							iif->raw[x + y] = 0xFF;
						else
							iif->raw[x + y] = g[x + y];
					} else
						iif->raw[x + y] = 0x00;
					break;
				case 2:
					if (g[x + y] > g[x + y - 1] && g[x + y] > g[x + y + 1]) {
						if (g[x + y] > 255)
							iif->raw[x + y] = 0xFF;
						else
							iif->raw[x + y] = g[x + y];
					} else
						iif->raw[x + y] = 0x00;
					break;
				case 3:
					if (g[x + y] > g[x + y - w + 1] && g[x + y] > g[x + y + w - 1]) {
						if (g[x + y] > 255)
							iif->raw[x + y] = 0xFF;
						else
							iif->raw[x + y] = g[x + y];
					} else
						iif->raw[x + y] = 0x00;
					break;
				default:
					printf("ERROR - direction outside range 0 to 3, dir[%d+%d] = %d\n", x, y, dir[x+y]);
					break;
			}
		}
	}
}

void calc_gradient_sobel(struct image_info *iif, int g[], int dir[]) {
	int w, h, x, y, max_x, max_y, g_x, g_y;
	float g_div;
	w = iif->width;
	h = iif->height;
	max_x = w - 3;
	max_y = w * (h - 3);
	for (y = w * 3; y < max_y; y += w) {
		for (x = 3; x < max_x; x++) {
			g_x = (2 * iif->raw[x + y + 1] 
				+ iif->raw[x + y - w + 1]
				+ iif->raw[x + y + w + 1]
				- 2 * iif->raw[x + y - 1] 
				- iif->raw[x + y - w - 1]
				- iif->raw[x + y + w - 1]);
			g_y = 2 * iif->raw[x + y - w] 
				+ iif->raw[x + y - w + 1]
				+ iif->raw[x + y - w - 1]
				- 2 * iif->raw[x + y + w] 
				- iif->raw[x + y + w + 1]
				- iif->raw[x + y + w - 1];
			#ifndef ABS_APPROX
			g[x + y] = sqrt(g_x * g_x + g_y * g_y);
			#endif
			#ifdef ABS_APPROX
			g[x + y] = abs(g_x[x + y]) + abs(g_y[x + y]);
			#endif
			if (g_x == 0) {
				dir[x + y] = 2;
			} else {
				g_div = g_y / (float) g_x;
				/* the following commented-out code is slightly
				 * faster than the code that follows, but is a
				 * slightly worse approximation for determining
				 * the edge direction angle
				if (g_div < 0) {
					if (g_div < -1) {
						dir[n] = 0;
					} else {
						dir[n] = 1;
					}
				} else {
					if (g_div > 1) {
						dir[n] = 0;
					} else {
						dir[n] = 3;
					}
				}
				*/
				if (g_div < 0) {
					if (g_div < -2.41421356237) {
						dir[x + y] = 0;
					} else {
						if (g_div < -0.414213562373) {
							dir[x + y] = 1;
						} else {
							dir[x + y] = 2;
						}
					}
				} else {
					if (g_div > 2.41421356237) {
						dir[x + y] = 0;
					} else {
						if (g_div > 0.414213562373) {
							dir[x + y] = 3;
						} else {
							dir[x + y] = 2;
						}
					}
				}
			}
		}
	}
}



void gaussian_noise_reduce(struct image_info *iif)
{
	int w, h, x, y, max_x, max_y;

	memset(iif->raw1, 0, iif->width * iif->height * iif->bytes_per_pixel);

	w = iif->width;
	h = iif->height;
	max_x = w - 2;
	max_y = w * (h - 2);
	for (y = w * 2; y < max_y; y += w) {
		for (x = 2; x < max_x; x++) {
			iif->raw1[x + y] =
				(2 * iif->raw[x + y - 2 - w - w] + 
				4 * iif->raw[x + y - 1 - w - w] + 
				5 * iif->raw[x + y - w - w] + 
				4 * iif->raw[x + y + 1 - w - w] + 
				2 * iif->raw[x + y + 2 - w - w] + 
				4 * iif->raw[x + y - 2 - w] + 
				9 * iif->raw[x + y - 1 - w] + 
				12 * iif->raw[x + y - w] + 
				9 * iif->raw[x + y + 1 - w] + 
				4 * iif->raw[x + y + 2 - w] + 
				5 * iif->raw[x + y - 2] + 
				12 * iif->raw[x + y - 1] + 
				15 * iif->raw[x + y] + 
				12 * iif->raw[x + y + 1] + 
				5 * iif->raw[x + y + 2] + 
				4 * iif->raw[x + y - 2 + w] + 
				9 * iif->raw[x + y - 1 + w] + 
				12 * iif->raw[x + y + w] + 
				9 * iif->raw[x + y + 1 + w] + 
				4 * iif->raw[x + y + 2 + w] + 
				2 * iif->raw[x + y - 2 + w + w] + 
				4 * iif->raw[x + y - 1 + w + w] + 
				5 * iif->raw[x + y + w + w] + 
				4 * iif->raw[x + y + 1 + w + w] + 
				2 * iif->raw[x + y + 2 + w + w]) / 159;
		}
	}
}





