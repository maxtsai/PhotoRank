#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jpeg.h"
#include "functions.h"

int main()
{
	struct image_info iif;
	char filename[128];
	int i;


	system("date");

	printf("\t--- sobel edge detect\n");
	for (i = 5; i < 8; i++) {
		sprintf(filename, "./samples/%02d.jpg", i);
		read_jpeg(filename, &iif, 1);
		sobel(&iif, 70);
		image_swap(&iif);
		printf("\t%s, blur rate = %f\n", filename, calc_blur(&iif));

		sprintf(filename, "./tmp/%02d_out.jpg", i);
		write_jpeg(filename, &iif);
		image_clean(&iif);
	}
	system("date");

	printf("\t--- canny edge detect\n");
	for (i = 5; i < 8; i++) {
		sprintf(filename, "./samples/%02d.jpg", i);
		memset(&iif, 0, sizeof(struct image_info));
		read_jpeg(filename, &iif, 1);
		gaussian_noise_reduce(&iif);
		image_swap(&iif);
		canny_edge_detect(&iif);
		image_swap(&iif);
		sprintf(filename, "./tmp/%02d_canny_out.jpg", i);
		write_jpeg(filename, &iif);
		image_clean(&iif);
	}
	system("date");

	printf("\n\n");

	return 0;
}
