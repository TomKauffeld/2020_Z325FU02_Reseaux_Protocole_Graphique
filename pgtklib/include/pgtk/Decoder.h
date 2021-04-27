#ifndef __PGTK_DECODER_H__
#define __PGTK_DECODER_H__
#include "PgtkLibApi.h"
#include "Image.h"


PGTK_API unsigned long long pgtk_decode_flat(const Image_t* image, void* buffer, unsigned long long buffer_size, unsigned char threshold);

PGTK_API unsigned long long pgtk_decode_resize(const Image_t* image, void* buffer, unsigned long long buffer_size, unsigned char threshold, unsigned int scale);

PGTK_API unsigned long long pgtk_decode(const Image_t* image, void* buffer, unsigned long long buffer_size);



#endif