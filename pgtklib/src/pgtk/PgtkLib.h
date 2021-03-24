#ifndef __INTERNAL__PGTK_PGTKLIB_H__
#define __INTERNAL__PGTK_PGTKLIB_H__

#include <pgtk/pgtklib.h>

bool pgtklib_ready();

void pgtklib_clear_error();

void pgtklib_set_error(int errno, const char* error);

#endif

