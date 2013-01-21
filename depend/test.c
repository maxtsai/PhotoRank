#include <stdio.h>
#include <stdlib.h>

#include "jpeg.h"

int main()
{
#define LOOP 1
	struct image_info iif;
	int i = 0, ret;

	ret = system("date");
	for (i = 0; i < LOOP; i++) {
		read_jpeg("./samples/01.jpg", &iif, 1);
		write_jpeg("./tmp/01_out.jpg", &iif);
		image_clean(&iif);
		
		read_jpeg("./samples/02.jpg", &iif, 2);
		write_jpeg("./tmp/02_out.jpg", &iif);
		image_clean(&iif);
	}
	printf("\t===> generate ./tmp/01_out.jpg\n");
	printf("\t===> generate ./tmp/02_out.jpg\n");

	ret = system("date");

	printf("\n\n");
	return 0;
}
