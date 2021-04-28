#include "Encoder.h"
#include "PgtkLib.h"
#include "Image.h"
#include <hc/HammingCode.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <pgtk/errors.h>
#include "pgtk_definitions.h"

unsigned char pgtk_encoder_read(unsigned char* data, unsigned long long data_size, unsigned long long block_size, unsigned long long block, unsigned long bit)
{
	unsigned long long i;
	unsigned char b;
	unsigned long long index = block_size * block + bit;
	if (index >= data_size)
		return 0ui8;
	i = index / 8ui64;
	b = 1 << (index % 8ui8);
	if ((((const unsigned char*)data)[i] & b) > 0)
		return 1ui8;
	return 0ui8;
}


Image_t* pgtk_encode_resize(void* data, unsigned long long size, unsigned int scale)
{
	Image_t* source;
	Image_t* destination;
	if (scale < 1)
	{
		pgtklib_set_error(PGTK_ERRNO_IMAGE_SCALE, "invalid scale");
		return NULL;
	}
	source = pgtk_encode(data, size);
	if (!source)
		return NULL;
	destination = image_resize(source, image_get_width(source) * scale, image_get_height(source) * scale);
	image_destroy(source);
	return destination;
}

Image_t* pgtk_encode(void* data, unsigned long long size)
{
	HammingEncodeResult_t result;
	unsigned char* buffer;
	unsigned int s;
	long double tmp_ld;
	int i;
	unsigned char b;
	unsigned char tmp_uc;
	unsigned char blocks;
	unsigned int x, y;
	Image_t* image;
	unsigned long long buffer_size = size * 2;
	if (!pgtklib_ready())
		return NULL;
	buffer = (char*)malloc(buffer_size);
	if (!buffer)
	{
		pgtklib_set_error(PGTK_ERRNO_MALLOC, "couldn't allocate memory");
		return NULL;
	}
	result = hc_encode(data, size * 8, buffer, size * 2 * 8, parity_odd, hamming_7_4, hc_mode_secded);
	if (result.status != hc_status_ok)
	{
		free(buffer);
		pgtklib_set_error(PGTK_ERRNO_ENCODING_HC, "couldn't encode data");
		return NULL;
	}
	if (result.blocks > UCHAR_MAX)
	{
		free(buffer);
		pgtklib_set_error(PGTK_ERRNO_ENCODING_SIZE, "too much data");
		return NULL;
	}
	blocks = (unsigned char)result.blocks;
	tmp_ld = sqrtl((long double)(result.bits + MARKER_WASTED));
	s = (unsigned int)ceill(tmp_ld);
	if (s < MARKER_SIZE * 2)
		s = MARKER_SIZE * 2;
	image = image_init(s, s);
	if (!image)
	{
		free(buffer);
		return NULL;
	}
	memset(image->data, 0xffui8, (size_t)s * s * 3);
	//TOP LEFT MARKER
	image_internal_set(image, 0, 0, 8, 0);
	image_internal_set(image, 2, 2, 4, 0);
	image_internal_set(image, 2, 3, 4, 0);
	image_internal_set(image, 2, 4, 2, 0);
	image_internal_set(image, 2, 5, 2, 0);
	image_internal_set(image, 0, 7, 8, 0);

	//TOP RIGHT MARKER
	image_internal_set(image, s - 8, 0, 8, 0);
	image_internal_set(image, s - 6, 2, 4, 0);
	image_internal_set(image, s - 6, 3, 4, 0);
	image_internal_set(image, s - 4, 4, 2, 0);
	image_internal_set(image, s - 4, 5, 2, 0);
	image_internal_set(image, s - 8, 7, 8, 0);

	//BOTTOM LEFT MARKER
	image_internal_set(image, 0, s - 8, 8, 0);
	image_internal_set(image, 2, s - 6, 2, 0);
	image_internal_set(image, 2, s - 5, 2, 0);
	image_internal_set(image, 2, s - 4, 4, 0);
	image_internal_set(image, 2, s - 3, 4, 0);
	image_internal_set(image, 0, s - 1, 8, 0);

	for (b = 1; b < 7; ++b)
	{
		image_internal_set(image, 0, b, 1, 0);
		image_internal_set(image, 7, b, 1, 0);
		image_internal_set(image, 0, s - b - 1, 1, 0);
		image_internal_set(image, 7, s - b - 1, 1, 0);
		image_internal_set(image, s - 1, b, 1, 0);
		image_internal_set(image, s - 8, b, 1, 0);
	}

	for (b = 0; b < 8; ++b)
	{
		tmp_uc = 1 << b;
		if ((PGTK_VERSION_1 & tmp_uc) > 0)
		{
			image_internal_set(image, 8, b, 1, 0);
			image_internal_set(image, b, s - 9, 1, 0);
			image_internal_set(image, s - b - 1, 8, 1, 0);
		}
		if ((blocks & tmp_uc) > 0)
		{
			image_internal_set(image, b, 8, 1, 0);
			image_internal_set(image, 8, s - b - 1, 1, 0);
			image_internal_set(image, s - 9, b, 1, 0);
		}
	}

	x = s - 1;
	y = s - 1;
	for (b = 0; b < result.n; ++b)
	{
		for (i = 0; i < result.blocks; ++i)
		{
			if (pgtk_encoder_read(buffer, result.bits, result.n, i, b))
				image_internal_set(image, x, y, 1, 0);
			if (y == 0 || (y <= MARKER_SIZE && (x < MARKER_SIZE || x >= s - MARKER_SIZE)))
			{
				if (x <= MARKER_SIZE)
					y = s - MARKER_SIZE - 1;
				else
					y = s - 1;
				--x;
			}
			else
				--y;
		}
	}
	free(buffer);
	return image;
}