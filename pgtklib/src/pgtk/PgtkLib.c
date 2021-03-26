#include "PgtkLib.h"
#include <pgtk/errors.h>

int pgtk_lib_errno;
bool pgtk_lib_ready = false;
const char* pgtk_lib_error = (char*)0;

bool pgtklib_init()
{
	if (pgtk_lib_ready)
		return true;
	pgtklib_clear_error();
	pgtk_lib_ready = true;
	return true;
}

bool pgtklib_ready()
{
	return pgtk_lib_ready;
}

int pgtklib_errno()
{
	return pgtk_lib_errno;
}

const char* pgtklib_error()
{
	return pgtk_lib_error;
}

void pgtklib_clear_error()
{
	pgtk_lib_errno = PGTK_ERRNO_NONE;
	pgtk_lib_error = (char*)0;
}

void pgtklib_set_error(int errno, const char* error)
{
	pgtk_lib_errno = errno;
	pgtk_lib_error = error;
}

int pgtklib_set_error_int(int errno, const char* error, int ret)
{
	pgtklib_set_error(errno, error);
	return ret;
}


void pgtklib_destroy()
{
	pgtk_lib_ready = false;
	return;
}