#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include "jpeg.h"
#include "functions.h"

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

#undef RESAMPLING

#ifdef RESAMPLING
static int resampling(struct image_info *iif, struct image_info *r_iif) {
	int width, height, ratio = 1;
	int i, j, x, y;
#define PIXELS 1200
#define MIN_PIXELS 1000

	width = iif->width;
	height = iif->height;

	if (width < MIN_PIXELS && height < MIN_PIXELS)
		return 0;
	ratio = (int) (width > height ? height / PIXELS : width / PIXELS);

	r_iif->width = width / ratio;
	r_iif->height = height / ratio;

	for (i = 0, y = 0; i < iif->height ; i += ratio, y++) {
		for (j = 0, x = 0; j < iif->width; j += ratio, x++) {
			r_iif->raw[x + r_iif->width * y] = iif->raw[j + iif->width * i];
		}
	}
	return 0;
}
#endif

static int _sobel(struct image_info *iif, double threshold) {

	int  x, y, i, v, u;
	unsigned char R, G, B;
	double val[MASK_N] = {0.0};
	int xadjust, yadjust, xbound, ybound;
	double total;

#ifdef RESAMPLING
	struct image_info r_iif;
	int width, height;
	r_iif.raw = iif->raw1;
	resampling(iif, &r_iif);
	width = iif->width;
	height = iif->height;
	iif->width = r_iif.width;
	iif->height = r_iif.height;
	image_swap(iif);
#endif
 
	xadjust = (MASK_X % 2) ? 1 : 0;
	yadjust = (MASK_Y % 2) ? 1 : 0;
	xbound = MASK_X / 2;
	ybound = MASK_Y / 2;


	for(y = 0; y != iif->height; ++y) {
		for(x = 0; x != iif->width; ++x) { 
			for(i = 0; i != MASK_N; ++i) {
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
				/* black */
				*(iif->raw1 + iif->bytes_per_pixel * (iif->width * y + x) + 2) = BLACK;
				*(iif->raw1 + iif->bytes_per_pixel * (iif->width * y + x) + 1) = BLACK;
				*(iif->raw1 + iif->bytes_per_pixel * (iif->width * y + x) + 0) = BLACK;
			}
			else {
				/* white */
				*(iif->raw1 + iif->bytes_per_pixel * (iif->width * y + x) + 2) = WHITE;
				*(iif->raw1 + iif->bytes_per_pixel * (iif->width * y + x) + 1) = WHITE;
				*(iif->raw1 + iif->bytes_per_pixel * (iif->width * y + x) + 0) = WHITE;
			}
		}
	}

	return 0;
}

#define CPU_CORE_NUM 4
struct _thread_data {
	struct image_info iif;
	double threshold;
};

static struct _thread_data g_thread_data[CPU_CORE_NUM];

static void *thread_func(void *parm) {
	struct _thread_data *td = (struct _thread_data*) parm;
	struct image_info *iif = &td->iif;
	double threshold = td->threshold;

	_sobel(iif, threshold);

	return NULL;
}

int parallel_sobel(struct image_info *iif, double threshold) {
	pthread_t threads[CPU_CORE_NUM];
	int i, ret;

	for (i = 0; i < CPU_CORE_NUM; i++) {
		g_thread_data[i].threshold = threshold;
		g_thread_data[i].iif.width = iif->width;
		g_thread_data[i].iif.height = iif->height / CPU_CORE_NUM;
		g_thread_data[i].iif.bytes_per_pixel = 1;
		g_thread_data[i].iif.raw =
			(unsigned char*) (iif->raw + g_thread_data[i].iif.width * g_thread_data[i].iif.height * i);
		g_thread_data[i].iif.raw1 =
			(unsigned char*) (iif->raw1 + g_thread_data[i].iif.width * g_thread_data[i].iif.height * i);
		pthread_create(&threads[i], NULL, thread_func, (void*) &g_thread_data[i]);
	}

	for (i = 0; i < CPU_CORE_NUM; i++) {
		ret = pthread_join(threads[i], NULL);

		/* partial blur rate
		image_swap(&g_thread_data[i].iif);
		printf("\t\tpart %d, blur rate = %f\n", i, calc_blur(&g_thread_data[i].iif));
		image_swap(&g_thread_data[i].iif);
		*/

	}

	return 0;
}



int sobel(struct image_info *iif, double threshold) {

	parallel_sobel(iif, threshold);

	/* _sobel(iif, threshold); */

	return 0;
}

