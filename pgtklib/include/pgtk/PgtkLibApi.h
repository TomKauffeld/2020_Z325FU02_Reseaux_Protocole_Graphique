#ifndef PGTK_API
#ifdef _EXPORT_PGTK_LIB
#define PGTK_API __declspec(dllexport)
#else
#define PGTK_API __declspec(dllimport)
#endif
#endif
