#include "Decoder.h"
#include "Image.h"
#include "PgtkLib.h"
#include "pgtk_definitions.h"
#include <pgtk/errors.h>
#include <hc/HammingCode.h>
#include <stdlib.h>
#include <string.h>


#define ORIENTATION_TOP_LEFT    0ui8
#define ORIENTATION_TOP_RIGHT   1ui8
#define ORIENTATION_BOTTOM_LEFT 2ui8

int pgtk_decoder_write(unsigned char* data, unsigned long long data_size, unsigned long long block_size, unsigned long long block, unsigned long long bit, unsigned char d)
{
	unsigned long long i;
	unsigned char b;
	unsigned long long index = block_size * block + bit;
	if (index >= data_size * 8)
		return -1;
	i = index / 8ui64;
	b = 1 << (index % 8ui8);
	if (d)
		((unsigned char*)data)[i] |= b;
	else
		((unsigned char*)data)[i] &= ~b;
	return 0;
}

unsigned long long pgtk_decode_v1(const Image_t* image, void* buffer, unsigned long long buffer_size, unsigned char threshold, unsigned char size)
{
	HammingDecodeResult_t result;
	unsigned int s;
	unsigned char b;
	unsigned int x, y;
	int i;
	unsigned char* tmp;
	unsigned char parity_bits = hamming_7_4;
	unsigned long long block_size = (1ui64 << parity_bits);
	unsigned long long tmp_size = size * block_size / 8 + 1;
	tmp = (unsigned char*)malloc(tmp_size);
	if (!tmp)
		return pgtklib_set_error_int(PGTK_ERRNO_MALLOC, "couldn't allocate memory", 0);
	s = image_get_width(image);
	x = s - 1;
	y = s - 1;
	for (b = 0; b < block_size; ++b)
	{
		for (i = 0; i < size; ++i)
		{
			if (image_get_brightness(image, x, y) < threshold)
				pgtk_decoder_write(tmp, tmp_size, block_size, i, b, 1);
			else
				pgtk_decoder_write(tmp, tmp_size, block_size, i, b, 0);
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
	result = hc_decode(tmp, block_size * size, buffer, buffer_size * 8, parity_odd, parity_bits, hc_mode_secded);
	free(tmp);
	if (result.status != hc_status_ok)
		return pgtklib_set_error_int(PGTK_ERRNO_ENCODING_HC, "couldn't decode the data", 0);
	return result.bytes;
}


int pgtk_decode_marker(const Image_t* image, unsigned int x, unsigned int y, unsigned char threshold, unsigned char orientation, unsigned char* size, unsigned char* version)
{
	unsigned int i, j, min_j, max_j;
	unsigned int offset_x = 0;
	unsigned int offset_y = 0;
	if ((orientation & ORIENTATION_TOP_RIGHT) > 0)
		++offset_x;
	if ((orientation & ORIENTATION_BOTTOM_LEFT) > 0)
		++offset_y;
	for (i = 0; i < MARKER_SIZE - 1; ++i)
	{
		if (image_get_brightness(image, x + offset_x + i,               y + offset_y + 0)               >= threshold)
			return 0;
		if (image_get_brightness(image, x + offset_x + i,               y + offset_y + MARKER_SIZE - 2) >= threshold)
			return 0;
		if (image_get_brightness(image, x + offset_x + 0,               y + offset_y + i)               >= threshold)
			return 0;
		if (image_get_brightness(image, x + offset_x + MARKER_SIZE - 2, y + offset_y + i)               >= threshold)
			return 0;
	}
	for (i = 1; i < MARKER_SIZE - 2; ++i)
	{
		min_j = 2;
		max_j = 6;
		switch (orientation)
		{
		case ORIENTATION_TOP_LEFT:
			if (i >= 4)
				max_j = 4;
			break;
		case ORIENTATION_BOTTOM_LEFT:
			if (i >= 4)
				min_j = 4;
			break;
		case ORIENTATION_TOP_RIGHT:
			if (i < 4)
				max_j = 4;
			break;
		default:
			return 0;
		}
		for (j = 1; j < MARKER_SIZE - 2; ++j)
		{
			if (image_get_brightness(image, x + offset_x + i, y + offset_y + j) < threshold)
			{
				if (i < 2 || i >= 6 || j < min_j || j >= max_j)
					return 0;
			}
			else if (i >= 2 && i < 6 && j >= min_j && j < max_j)
			{
				return 0;
			}
		}
	}
	*version = 0;
	*size = 0;
	for (i = 0; i < 8; ++i)
	{
		switch (orientation)
		{
		case ORIENTATION_TOP_LEFT:
			if (image_get_brightness(image, x + MARKER_SIZE - 1, y + i) < threshold)
				*version |= 1 << i;
			if (image_get_brightness(image, x + i, y + MARKER_SIZE - 1) < threshold)
				*size |= 1 << i;
			break;
		case ORIENTATION_BOTTOM_LEFT:
			if (image_get_brightness(image, x + i, y) < threshold)
				*version |= 1 << i;
			if (image_get_brightness(image, x + MARKER_SIZE - 1, y + MARKER_SIZE - i - 1) < threshold)
				*size |= 1 << i;
			break;
		case ORIENTATION_TOP_RIGHT:
			if (image_get_brightness(image, x + MARKER_SIZE - i - 1, y + MARKER_SIZE - 1) < threshold)
				*version |= 1 << i;
			if (image_get_brightness(image, x, y + i) < threshold)
				*size |= 1 << i;
			break;
		default:
			return 0;
		}
	}
	return 1;
}

unsigned long long pgtk_decode_flat(const Image_t* image, void* buffer, unsigned long long buffer_size, unsigned char threshold)
{
	unsigned int s;
	unsigned char version, version_1, version_2, version_3;
	unsigned char size, size_1, size_2, size_3;
	if (image_get_height(image) != image_get_width(image))
		return pgtklib_set_error_int(PGTK_ERRNO_INVALID_IMAGE, "image not square", 0);
	s = image_get_width(image);
	if (s < MARKER_SIZE * 2)
		return pgtklib_set_error_int(PGTK_ERRNO_INVALID_IMAGE, "image too small", 0);


	if (!pgtk_decode_marker(image, 0, 0, threshold, ORIENTATION_TOP_LEFT, &size_1, &version_1))
		return pgtklib_set_error_int(PGTK_ERRNO_INVALID_IMAGE, "missing marker", 0);

	if (!pgtk_decode_marker(image, 0, s - MARKER_SIZE, threshold, ORIENTATION_BOTTOM_LEFT, &size_2, &version_2))
		return pgtklib_set_error_int(PGTK_ERRNO_INVALID_IMAGE, "missing marker", 0);

	if (!pgtk_decode_marker(image, s - MARKER_SIZE, 0, threshold, ORIENTATION_TOP_RIGHT, &size_3, &version_3))
		return pgtklib_set_error_int(PGTK_ERRNO_INVALID_IMAGE, "missing marker", 0);


	if (size_1 == size_2 || size_1 == size_3)
		size = size_1;
	else if (size_2 == size_3)
		size = size_2;
	else
		return pgtklib_set_error_int(PGTK_ERRNO_INVALID_IMAGE, "invalid marker", 0);

	if (version_1 == version_2 || version_1 == version_3)
		version = version_1;
	else if (version_2 == version_3)
		version = version_2;
	else
		return pgtklib_set_error_int(PGTK_ERRNO_INVALID_IMAGE, "invalid marker", 0);

	switch (version)
	{
	case PGTK_VERSION_1:
		return pgtk_decode_v1(image, buffer, buffer_size, threshold, size);
	default:
		return pgtklib_set_error_int(PGTK_ERRNO_INVALID_IMAGE, "invalid version", 0);
	}
}

unsigned long long pgtk_decode_resize(const Image_t* image, void* buffer, unsigned long long buffer_size, unsigned char threshold, unsigned int scale)
{
	Image_t* tmp;
	unsigned long long ret;
	if (scale < 1)
	{
		pgtklib_set_error(PGTK_ERRNO_IMAGE_SCALE, "invalid scale");
		return -1;
	}
	tmp = image_resize(image, image_get_width(image) / scale, image_get_height(image) / scale);
	if (!tmp)
		return 0;
	ret = pgtk_decode_flat(tmp, buffer, buffer_size, threshold);
	image_destroy(tmp);
	return ret;
}

unsigned long long pgtk_decode(const Image_t* image, void* buffer, unsigned long long buffer_size)
{
	unsigned char threshold = UCHAR_MAX / 2;
	unsigned scale = 1;
	while (image_get_brightness(image, scale, scale) < threshold)
		++scale;
	return pgtk_decode_resize(image, buffer, buffer_size, threshold, scale);
}