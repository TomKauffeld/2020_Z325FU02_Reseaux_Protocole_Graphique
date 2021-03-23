#ifndef HC_API
#ifdef _EXPORT_HAMMINGCODE_LIB
#define HC_API __declspec(dllexport)
#else
#define HC_API __declspec(dllimport)
#endif
#endif
