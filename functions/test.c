#include <stdio.h>
#include <stdlib.h>

#include "jpeg.h"
#include "functions.h"

int main()
{
#define LOOP 1
	struct image_info iif;
	int i;

	system("date");

		read_jpeg("./samples/03.jpg", &iif, 1);
	for (i = 0 ; i < LOOP; i++) {
		sobel(&iif, 50);
	}
		image_swap(&iif);
		write_jpeg("./tmp/03_out.jpg", &iif);
		image_clean(&iif);
	printf("\t==> generate ./tmp/03_out.jpg\n");

	system("date");

		read_jpeg("./samples/04.jpg", &iif, 1);
	for (i = 0 ; i < LOOP; i++) {
		gaussian_noise_reduce(&iif);
	}
		image_swap(&iif);
		/* write_jpeg("./tmp/04_gau_out.jpg", &iif); */
	for (i = 0 ; i < LOOP; i++) {
		canny_edge_detect(&iif);
	}
		image_swap(&iif);
		write_jpeg("./tmp/04_out.jpg", &iif);
	printf("\t==> generate ./tmp/04_out.jpg\n");
	image_clean(&iif);

	system("date");

	printf("\n\n");

	return 0;
}
