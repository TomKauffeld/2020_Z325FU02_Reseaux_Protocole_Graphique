#include "Image.h"
#include <stdlib.h>
#include <string.h>
#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"
#include "../stb/stb_image_resize.h"
#include "PgtkLib.h"
#include <pgtk/errors.h>

const int RGB = 3;
const int JPEG_QUALITY = 50;

Image_t* image_init(unsigned int width, unsigned int height)
{
	Image_t* image = (Image_t*)malloc(sizeof(Image_t));
	if (!image)
	{

		pgtklib_set_error(PGTK_ERRNO_MALLOC, "couldn't allocate memory for the image");
		return NULL;
	}
	image->width = width;
	image->height = height;
	image->data = (Color_t*)malloc(sizeof(Color_t) * width * height);
	if (!image->data)
	{
		pgtklib_set_error(PGTK_ERRNO_MALLOC, "couldn't allocate memory for the image data");
		free(image);
		return NULL;
	}
	return image;
}

Image_t* image_load(const char* filepath)
{
	Image_t* image;
	int width, height, channels;
	stbi_uc* uc;
	if (!pgtklib_ready())
		return NULL;
	uc = stbi_load(filepath, &width, &height, &channels, RGB);
	if (!uc)
	{
		pgtklib_set_error(PGTK_ERRNO_STB, stbi_failure_reason());
		return NULL;
	}
	image = image_init(width, height);
	if (!image)
	{
		stbi_image_free(uc);
		return NULL;
	}
	memcpy(image->data, uc, (size_t)RGB * width * height);
	stbi_image_free(uc);
	return image;
}

Image_t* image_resize(const Image_t* source, unsigned int new_width, unsigned int new_height)
{
	int res;
	Image_t* destination = image_init(new_width, new_height);
	res = stbir_resize_uint8(source->data, source->width, source->height, 0, destination->data, new_width, new_height, 0, RGB);
	if (!res)
	{
		pgtklib_set_error(PGTK_ERRNO_STB, "couldn't resize");
		image_destroy(destination);
		return NULL;
	}
	return destination;
}

int image_save(const Image_t* image, const char* filepath)
{
	char ext[4] = { 0, 0, 0, 0 };
	size_t length = strlen(filepath);
	if (length < 3)
		return 0;
	ext[0] = filepath[length - 3];
	ext[1] = filepath[length - 2];
	ext[2] = filepath[length - 1];
	if (strcmp(ext, "png") == 0)
		return stbi_write_png(filepath, image->width, image->height, RGB, image->data, image->width * RGB);
	if (strcmp(ext, "bmp") == 0)
		return stbi_write_bmp(filepath, image->width, image->height, RGB, image->data);
	if (strcmp(ext, "jpg") == 0)
		return stbi_write_jpg(filepath, image->width, image->height, RGB, image->data, JPEG_QUALITY);
	return 0;
}

unsigned int image_get_width(const Image_t* image)
{
	return image->width;
}

unsigned int image_get_height(const Image_t* image)
{
	return image->height;
}

Color_t image_get(const Image_t* image, unsigned int x, unsigned int y)
{
	Color_t c = { 0, 0, 0 };
	if (x > image->width || y >= image->height)
		return c;
	return image->data[x + y * image->width];
}

unsigned char image_get_r(const Image_t* image, unsigned int x, unsigned int y)
{
	return image_get(image, x, y).r;
}

unsigned char image_get_g(const Image_t* image, unsigned int x, unsigned int y)
{
	return image_get(image, x, y).g;
}

unsigned char image_get_b(const Image_t* image, unsigned int x, unsigned int y)
{
	return image_get(image, x, y).b;
}

void image_set(Image_t* image, unsigned int x, unsigned int y, Color_t c)
{
	if (x >= image->width || y >= image->height)
		return;
	image->data[x + y * image->width].r = c.r;
	image->data[x + y * image->width].g = c.g;
	image->data[x + y * image->width].b = c.b;
}

void image_set_r(Image_t* image, unsigned int x, unsigned int y, unsigned char r)
{
	if (x >= image->width || y >= image->height)
		return;
	image->data[x + y * image->width].r = r;
}

void image_set_g(Image_t* image, unsigned int x, unsigned int y, unsigned char g)
{
	if (x >= image->width || y >= image->height)
		return;
	image->data[x + y * image->width].g = g;
}

void image_set_b(Image_t* image, unsigned int x, unsigned int y, unsigned char b)
{
	if (x >= image->width || y >= image->height)
		return;
	image->data[x + y * image->width].b = b;
}

void image_destroy(Image_t* image)
{
	free(image->data);
	free(image);
}

void image_internal_set(Image_t* image, unsigned int x, unsigned int y, unsigned int length, unsigned char c)
{
	if (x + length > image->width || y >= image->height || length < 1)
		return;
	memset(image->data + x + (unsigned long long)y * image->width, c, (size_t)length * RGB);
}
