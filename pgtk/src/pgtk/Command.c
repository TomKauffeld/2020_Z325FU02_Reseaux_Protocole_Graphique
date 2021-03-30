#include "Command.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READ_MODE_DEFAULT 0
#define READ_MODE_INPUT 1
#define READ_MODE_OUTPUT 2

Command_t* command_init()
{
	Command_t* c = (Command_t*)malloc(sizeof(Command_t));
	if (!c)
		return NULL;
	c->mode = COMMAND_MODE_INVALID;
	c->loc = COMMAND_LOC_INVALID;
	c->input = NULL;
	c->output = NULL;
	return c;
}



Command_t* command_parse(int argc, char* argv[])
{
	int i, mode, ok, tmp;
	size_t len;
	Command_t* c;
	c = command_init();
	if (!c)
		return NULL;
	mode = READ_MODE_DEFAULT;
	ok = 1;
	tmp = -1;
	c->loc = COMMAND_LOC_FILE;
	c->scale = 10;
	for (i = 1; i < argc && ok; ++i)
	{
		len = strlen(argv[i]);
		switch (mode)
		{
		case READ_MODE_DEFAULT:
			if (strcmp(argv[i], "encode") == 0 && c->mode == COMMAND_MODE_INVALID)
				c->mode = COMMAND_MODE_ENCODE;
			else if (strcmp(argv[i], "decode") == 0 && c->mode == COMMAND_MODE_INVALID)
				c->mode = COMMAND_MODE_DECODE;
			else if (strcmp(argv[i], "-o") == 0 && !c->output)
				mode = READ_MODE_OUTPUT;
			else if (strcmp(argv[i], "-i") == 0 && !c->input)
				mode = READ_MODE_INPUT;
			else if (strcmp(argv[i], "-c") == 0)
			{
				c->loc = COMMAND_LOC_CONSOLE;
				tmp = i + 1;
			}
			else if (tmp != i && tmp >= 0)
			{
				printf("ERROR, unexpected token : %s\n", argv[i]);
				ok = 0;
			}
			break;
		case READ_MODE_INPUT:
			c->input = (char*)malloc(len + 1);
			if (!c->input)
			{
				printf("ERROR, couldn't allocate memory for the input\n");
				ok = 0;
			}
			else
			{
				c->input[len] = 0;
				strcpy(c->input, argv[i]);
			}
			mode = READ_MODE_DEFAULT;
			break;
		case READ_MODE_OUTPUT:
			c->output = (char*)malloc(len + 1);
			if (!c->output)
			{
				printf("ERROR, couldn't allocate memory for the output\n");
				ok = 0;
			}
			else
			{
				c->output[len] = 0;
				strcpy(c->output, argv[i]);
			}
			mode = READ_MODE_DEFAULT;
			break;
		default:
			printf("ERROR, invalid read mode\n");
			ok = 0;
			break;
		}
	}
	if (ok && c->loc == COMMAND_LOC_CONSOLE)
	{
		switch (c->mode)
		{
		case COMMAND_MODE_DECODE:
			ok = !c->output;
			if (!ok)
				printf("ERROR, -c and -o cannot be used together when decoding\n");
			break;
		case COMMAND_MODE_ENCODE:
			if (c->input)
			{
				printf("ERROR, -c and -i cannot be used together when encoding\n");
				ok = 0;
			}
			else
			{
				len = strlen(argv[tmp]);
				c->input = (char*)malloc(len + 1);
				if (!c->input)
				{
					printf("ERROR, couldn't allocate memory for the input\n");
					ok = 0;
				}
				else
				{
					c->input[len] = 0;
					strcpy(c->input, argv[tmp]);
				}
			}
			break;
		default:
			break;
		}
	}
	if (ok && ((c->mode == COMMAND_MODE_ENCODE && !c->output) || (c->mode == COMMAND_MODE_DECODE && !c->output && c->loc != COMMAND_LOC_CONSOLE)))
		ok = 0;
	if (!ok || c->mode == COMMAND_MODE_INVALID || !c->input)
	{
		command_destroy(c);
		c = NULL;
	}
	return c;
}


void command_destroy(Command_t* command)
{
	if (command->input)
		free(command->input);
	if (command->output)
		free(command->output);
	free(command);
}