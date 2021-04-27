#ifndef __PGTK_IMAGE_H__
#define __PGTK_IMAGE_H__

#include "PgtkLibApi.h"

typedef struct image Image_t;

typedef struct color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} Color_t;

PGTK_API Image_t* image_init(unsigned int width, unsigned int height);

PGTK_API Image_t* image_load(const char* filepath);

PGTK_API Image_t* image_resize(const Image_t* source, unsigned int new_width, unsigned int new_height);

PGTK_API int image_save(const Image_t* image, const char* filepath);

PGTK_API unsigned int image_get_width(const Image_t* image);
PGTK_API unsigned int image_get_height(const Image_t* image);

PGTK_API Color_t image_get(const Image_t* image, unsigned int x, unsigned int y);

PGTK_API unsigned char image_get_r(const Image_t* image, unsigned int x, unsigned int y);
PGTK_API unsigned char image_get_g(const Image_t* image, unsigned int x, unsigned int y);
PGTK_API unsigned char image_get_b(const Image_t* image, unsigned int x, unsigned int y);

PGTK_API unsigned char image_get_brightness(const Image_t* image, unsigned int x, unsigned int y);

PGTK_API void image_set_r(Image_t* image, unsigned int x, unsigned int y, unsigned char r);
PGTK_API void image_set_g(Image_t* image, unsigned int x, unsigned int y, unsigned char g);
PGTK_API void image_set_b(Image_t* image, unsigned int x, unsigned int y, unsigned char b);

PGTK_API void image_set(Image_t* image, unsigned int x, unsigned int y, Color_t c);

PGTK_API void image_destroy(Image_t* image);


#endif