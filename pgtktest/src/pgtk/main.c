#include <pgtk/PgtkLib.h>
#include <pgtk/Encoder.h>
#include <pgtk/Decoder.h>
#include <stdio.h>
#include <string.h>

int main()
{
	const int scale = 20;
	if (!pgtklib_init())
		return -1;
	char data[] = "Hello World!";
	unsigned long size = sizeof(data);
	char buffer[sizeof(data)+1];

	Image_t* image = pgtk_encode_resize(data, size, 10);
	if (!image)
	{
		printf("%s\n", pgtklib_error());
		pgtklib_destroy();
		return 1;
	}
	if (!image_save(image, "test.bmp"))
	{
		image_destroy(image);
		pgtklib_destroy();
		return 2;
	}
	image_destroy(image);
	image = image_load("test.bmp");
	if (!image)
	{
		printf("%s\n", pgtklib_error());
		pgtklib_destroy();
		return 3;
	}
	if (pgtk_decode(image, buffer, size) == 0)
	{
		printf("%s\n", pgtklib_error());
		image_destroy(image);
		pgtklib_destroy();
		return 4;
	}
	image_destroy(image);
	buffer[size] = 0;
	printf("%s\n", buffer);
	pgtklib_destroy();
	return strcmp(buffer, data) == 0 ? 0 : 5;
}