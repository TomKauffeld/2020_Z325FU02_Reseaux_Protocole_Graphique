#ifndef __PGTK_COMMAND_H__
#define __PGTK_COMMAND_H__

typedef unsigned char command_mode_t;
typedef unsigned char command_loc_t;

#define COMMAND_MODE_INVALID 0ui8
#define COMMAND_MODE_ENCODE 1ui8
#define COMMAND_MODE_DECODE 2ui8

#define COMMAND_LOC_INVALID 0ui8
#define COMMAND_LOC_FILE 1ui8
#define COMMAND_LOC_CONSOLE 2ui8

typedef struct command{
	command_mode_t mode;
	command_loc_t loc;
	char* input;
	char* output;
	unsigned int scale;
} Command_t;


Command_t* command_init();
Command_t* command_parse(int argc, char* argv[]);


void command_destroy(Command_t* command);

#endif