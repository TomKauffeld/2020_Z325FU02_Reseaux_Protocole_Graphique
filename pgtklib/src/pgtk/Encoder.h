#ifndef __INTERNAL__PGTK_ENCODER_H__
#define __INTERNAL__PGTK_ENCODER_H__
#include <pgtk/Encoder.h>

unsigned char pgtk_encoder_read(unsigned char* data, unsigned long long data_size, unsigned long long block_size, unsigned long long block, unsigned long bit);

#endif