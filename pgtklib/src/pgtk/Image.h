#ifndef __INTERNAL__PGTK_IMAGE_H__
#define __INTERNAL__PGTK_IMAGE_H__
#include <pgtk/Image.h>

struct image {
	unsigned int width;
	unsigned int height;
	Color_t* data;
};


void image_internal_set(Image_t* image, unsigned int x, unsigned int y, unsigned int length, unsigned char c);


#endif
