#include <stdio.h>
#include <stdlib.h>

#include "jpeg.h"
#include "functions.h"

int main()
{
	struct image_info iif;

	system("date");

	read_jpeg("./samples/03.jpg", &iif, 2);
	sobel(&iif, 50);
	image_swap(&iif);
	write_jpeg("./tmp/03_out.jpg", &iif);
	image_clean(&iif);
	printf("\t==> generate ./tmp/03_out.jpg\n");

	system("date");

	printf("\n\n");

	return 0;
}
