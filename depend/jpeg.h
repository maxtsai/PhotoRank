#ifndef __JPEG_H__
#define __JPEG_H__

#include "core.h"

int read_jpeg(char *filename, struct image_info *iif, int desired_color_space);
int write_jpeg(char *filename, struct image_info *iif);

#endif
