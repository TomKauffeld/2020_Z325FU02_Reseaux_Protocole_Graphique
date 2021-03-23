#include "PgtkLib.h"
#include <pgtk/errors.h>

int pgtk_lib_errno;
bool pgtk_lib_ready = FALSE;


bool pgtklib_init()
{
	if (pgtk_lib_ready)
		return TRUE;
	pgtk_lib_errno = PGTK_ERRNO_NONE;
	pgtk_lib_ready = TRUE;
	return TRUE;
}

int pgtklib_errno()
{
	return pgtk_lib_errno;
}

void pgtklib_set_errno(int errno)
{
	pgtk_lib_errno = errno;
}



void pgtklib_destroy()
{
	pgtk_lib_ready = FALSE;
	return;
}