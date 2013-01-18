#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "jpeg.h"

#define MASK_N 2
#define MASK_X 3
#define MASK_Y 3
#define WHITE  255
#define BLACK  0

int mask[MASK_N][MASK_X][MASK_Y] = {
	{
		{-1,-2,-1},
		{0 , 0, 0},
		{1 , 2, 1}
	},
	{
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	}
};

int sobel(struct image_info *iif, double threshold) {

	unsigned int  x, y, i, v, u;
	unsigned char R, G, B;
	double val[MASK_N] = {0.0};
	int xadjust, yadjust, xbound, ybound;
	double total;
 
	for(y = 0; y != iif->height; ++y) {
		for(x = 0; x != iif->width; ++x) { 
			for(i = 0; i != MASK_N; ++i) {
				xadjust = (MASK_X % 2) ? 1 : 0;
				yadjust = (MASK_Y % 2) ? 1 : 0;
				xbound = MASK_X / 2;
				ybound = MASK_Y / 2;
				val[i] = 0.0;
				for(v = -ybound; v != ybound + yadjust; ++v) {
					for (u = -xbound; u != xbound + xadjust; ++u) {
						if (x + u >= 0 && x + u < iif->width && y + v >= 0 && y + v < iif->height) {
							R = *(iif->raw + iif->bytes_per_pixel * (iif->width * (y+v) + (x+u)) + 2);
							G = *(iif->raw + iif->bytes_per_pixel * (iif->width * (y+v) + (x+u)) + 1);
							B = *(iif->raw + iif->bytes_per_pixel * (iif->width * (y+v) + (x+u)) + 0);
							val[i] += ((R + G + B) / 3 * mask[i][u + xbound][v + ybound]);
						}
					}
				}
			}

			total = 0.0;
			for (i = 0; i != MASK_N; ++i) {
				total += val[i] * val[i];
			}
			total = sqrt(total);
			if (total - threshold >= 0) {
				// black
				*(iif->m_raw + iif->bytes_per_pixel * (iif->width * y + x) + 2) = BLACK;
				*(iif->m_raw + iif->bytes_per_pixel * (iif->width * y + x) + 1) = BLACK;
				*(iif->m_raw + iif->bytes_per_pixel * (iif->width * y + x) + 0) = BLACK;
			}
			else {
				// white
				*(iif->m_raw + iif->bytes_per_pixel * (iif->width * y + x) + 2) = WHITE;
				*(iif->m_raw + iif->bytes_per_pixel * (iif->width * y + x) + 1) = WHITE;
				*(iif->m_raw + iif->bytes_per_pixel * (iif->width * y + x) + 0) = WHITE;
			}
		}
	}
	return 0;
}
