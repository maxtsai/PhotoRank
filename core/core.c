#include <stdlib.h>
#include "core.h"

int image_create(struct image_info *iif)
{
	if (!iif)
		return -1;
	if (iif->width <= 0 || iif->height <= 0)
		return -1;

	if (iif->raw)
		free(iif->raw);
	if (iif->raw1)
		free(iif->raw1);

	iif->raw = (unsigned char*) malloc(iif->width * iif->height * iif->bytes_per_pixel);
	if (iif->raw == NULL)
		return -1;

	iif->raw1 = (unsigned char*) malloc(iif->width * iif->height * iif->bytes_per_pixel);
	if (iif->raw1 == NULL) {
		free(iif->raw);
		return -1;
	}

	return 0;
}

void image_clean(struct image_info *iif)
{
	if (!iif)
		return;
	if (iif->raw) {
		free(iif->raw);
		iif->raw = NULL;
	}
	if (iif->raw1) {
		free(iif->raw1);
		iif->raw1 = NULL;
	}
}

void image_swap(struct image_info *iif)
{
	unsigned char *tmp;

	if (!iif)
		return;

	tmp = iif->raw1;
	iif->raw1 = iif->raw;
	iif->raw = tmp;
}
