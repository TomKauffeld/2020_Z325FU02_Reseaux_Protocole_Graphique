#ifndef __INTERNAL__PGTK_PGTKLIB_H__
#define __INTERNAL__PGTK_PGTKLIB_H__

#include <pgtk/pgtklib.h>

bool pgtklib_ready();

void pgtklib_clear_error();

void pgtklib_set_error(int errno, const char* error);

int pgtklib_set_error_int(int errno, const char* error, int ret);

#endif

