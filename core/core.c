#include <stdlib.h>
#include "core.h"


void image_clean(struct image_info *iif)
{
	if (!iif)
		return;
	if (iif->raw) {
		free(iif->raw); iif->raw = NULL;
	}
	if (iif->raw1) {
		free(iif->raw1); iif->raw1 = NULL;
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
