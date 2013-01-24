
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "core.h"


static int get_edge_strength(struct image_info *iif, double *edges)
{
	int x, y;

	if (!iif || !edges)
		return -1;

	for (x = 0; x < iif->width; x++) {
		for (y = 0; y < iif->height; y++) {
			edges[x + iif->width * y] = iif->raw[x + iif->width * y] / 255;
		}
	}

	return 0;
}

static double calc_kurtosis(double *edges, int len)
{
	double kurt = 0, accum = 0;
	long double sum = 0, sumdist = 0;
	float mean = 0;
	int i;

	if (!edges || len < 4) {
		printf("%s: invalid input arguments\n", __FUNCTION__);
		abort();
	}

	for (i = 0; i < len; i++)
		sum += edges[i];

	mean = (float) (sum / len);

	for (i = 0; i < len; i++) {
		accum += pow((edges[i] - mean), 4.0);
		sumdist += pow((edges[i] - mean), 2.0);
	}
	sumdist = pow(sumdist, 2.0);

	kurt =
		(len * (len + 1) * (len - 1) * accum) / ((len - 2) * ( len  - 3) * sumdist);

	return kurt;
}

/* precondition:
 * 	Image should be transfered to grayscale in advance
 */

float calc_blur(struct image_info *iif)
{
	float rate = 0;
	double *edges = malloc(iif->width * iif->height * sizeof(double));
	
	get_edge_strength(iif, edges);
	rate = calc_kurtosis(edges, iif->width * iif->height) * 100000.f;

	free(edges);

	return (rate > 100 ? 100 : rate);
}

