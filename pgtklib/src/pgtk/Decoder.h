#ifndef __INTERNAL__PGTK_DECODER_H__
#define __INTERNAL__PGTK_DECODER_H__
#include <pgtk/Decoder.h>

int pgtk_decoder_write(unsigned char* data, unsigned long long data_size, unsigned long long block_size, unsigned long long block, unsigned long long bit, unsigned char d);

unsigned long long pgtk_decode_v1(const Image_t* image, void* buffer, unsigned long long buffer_size, unsigned char threshold, unsigned char size);

int pgtk_decode_marker(const Image_t* image, unsigned int x, unsigned int y, unsigned char threshold, unsigned char orientation, unsigned char* size, unsigned char* version);

#endif