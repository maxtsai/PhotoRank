/*
	FAST-EDGE
	Copyright (c) 2009 Benjamin C. Haynor

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _FASTCANNY
#define _FASTCANNY

/* percentage of the high threshold value that the low threshold shall be set at */
#define LOW_THRESHOLD_PERCENTAGE 0.8
#define PI 3.14159265

/* percentage of pixels that meet the high threshold - for example 0.15 will ensure
 * that at least 15% of edge pixels are considered to meet the high threshold */
#define HIGH_THRESHOLD_PERCENTAGE 0.15

#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) < (Y) ? (Y) : (X))

/* define to use the absolute value approximation of sqrt(Gx ^ 2 + Gy ^2) */
#undef ABS_APPROX

/* define to print the histogram used to estimate the threshold */
#undef PRINT_HISTOGRAM

void canny_edge_detect(struct image_info *iif);
void calc_gradient_sobel(struct image_info *iif, int g[], int dir[]);
void non_max_suppression(struct image_info *iif, int g[], int dir[]);
void estimate_threshold(struct image_info *iif, int *high, int *low);
void hysteresis (int high, int low, struct image_info *img_in, struct image_info *img_out);
int trace (int x, int y, int low, struct image_info *img_in, struct image_info *img_out);
int range (struct image_info *img, int x, int y);
#endif
