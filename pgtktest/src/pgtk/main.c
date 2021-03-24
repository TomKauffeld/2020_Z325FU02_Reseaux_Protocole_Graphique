#include <pgtk/PgtkLib.h>
#include <pgtk/Encoder.h>
#include <stdio.h>
#include <string.h>

int main()
{
	const int scale = 20;
	int res;
	if (!pgtklib_init())
		return -1;
	char data[] = "Hello World!";
	unsigned long size = sizeof(data);
	
	Image_t* image = pgtk_encode(data, size);
	if (!image)
	{
		printf("%s\n", pgtklib_error());
		pgtklib_destroy();
		return 1;
	}
	Image_t* bigger = image_resize(image, image_get_width(image) * scale, image_get_height(image) * scale);
	image_destroy(image);
	if (!bigger)
	{
		printf("%s\n", pgtklib_error());
		pgtklib_destroy();
		return 2;
	}
	if (image_save(bigger, "test.bmp"))
		res = 0;
	else
		res = 3;
	image_destroy(bigger);
	pgtklib_destroy();
	return res;
}