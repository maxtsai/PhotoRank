#include <stdio.h>
#include <stdlib.h>

#include "jpeglib.h"

int main()
{
	struct image_info iif;

	read_jpeg("./samples/01.jpg", &iif, 1);
	write_jpeg("./tmp/01_out.jpg", &iif);
	read_jpeg("./samples/02.jpg", &iif, 2);
	write_jpeg("./tmp/02_out.jpg", &iif);

	printf("\t===> generate ./tmp/01_out.jpg\n");
	printf("\t===> generate ./tmp/02_out.jpg\n");

	return 0;
}
