#include "Command.h"
#include <pgtk/Image.h>
#include <pgtk/PgtkLib.h>
#include <stdio.h>
#include <string.h>
#include <pgtk/Decoder.h>
#include <pgtk/Encoder.h>

#define DATA_SIZE 1028


int encode(Command_t* c)
{
	FILE* f;
	unsigned long long bytes;
	Image_t* image;
	unsigned char data[DATA_SIZE];
	switch (c->loc)
	{
	case COMMAND_LOC_CONSOLE:
		bytes = strlen(c->input) + 1;
		if (bytes >= DATA_SIZE)
		{
			printf("ERROR buffer too small\n");
			return 0;
		}
		strcpy(data, c->input);
		break;
	case COMMAND_LOC_FILE:
		f = fopen(c->input, "rb");
		if (!f)
		{
			printf("ERROR reading the file\n");
			return 0;
		}
		bytes = fread(data, 1, DATA_SIZE, f);
		fclose(f);
		break;
	default:
		printf("ERROR invalid loc mode : %x\n", c->loc);
		return 0;
	}
	image = pgtk_encode_resize(data, bytes, c->scale);
	if (!image)
	{
		printf("ERROR encoding the data : %s\n", pgtklib_error());
		return 0;
	}
	image_save(image, c->output);
	image_destroy(image);
	return 1;
}

int decode(Command_t* c)
{
	FILE* f;
	unsigned long long bytes;
	Image_t* image;
	unsigned char data[DATA_SIZE];
	image = image_load(c->input);
	if (!image)
	{
		printf("ERROR loading the image : %s\n", pgtklib_error());
		return 0;
	}
	bytes = pgtk_decode(image, data, DATA_SIZE);
	image_destroy(image);
	if (bytes < 1)
	{
		printf("ERROR decoding the data : %s\n", pgtklib_error());
		return 0;
	}
	switch (c->loc)
	{
	case COMMAND_LOC_CONSOLE:
		printf("%s\n", data);
		break;
	case COMMAND_LOC_FILE:
		f = fopen(c->output, "wb");
		if (!f)
		{
			printf("ERROR writing the file\n");
			return 0;
		}
		fwrite(data, 1, bytes, f);
		fclose(f);
		break;
	}
	return 1;
}


int main(int argc, char* argv[])
{
	int ret;
	Command_t* c;
	pgtklib_init();
	c = command_parse(argc, argv);
	if (!c)
	{
		printf("ERROR couldnt parse the arguments\n");
		pgtklib_destroy();
		return -1;
	}
	switch (c->mode)
	{
	case COMMAND_MODE_DECODE:
		ret = decode(c);
		break;
	case COMMAND_MODE_ENCODE:
		ret = encode(c);
		break;
	default:
		ret = 0;
		break;
	}
	command_destroy(c);
	pgtklib_destroy();
	if (ret)
		return 0;
	return 1;
}