#ifndef __PGTK_PGTKLIB_H__
#define __PGTK_PGTKLIB_H__

#include "PgtkLibApi.h"
#include "../utils/Bool.h"

PGTK_API bool pgtklib_init();

PGTK_API int pgtklib_errno();

PGTK_API const char* pgtklib_error();

PGTK_API void pgtklib_destroy();

#endif
