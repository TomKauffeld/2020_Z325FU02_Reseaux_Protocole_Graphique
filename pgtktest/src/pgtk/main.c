#include <pgtk/PgtkLib.h>

int main()
{
	if (!pgtklib_init())
		return 1;
	

	pgtklib_destroy();
	return 0;
}