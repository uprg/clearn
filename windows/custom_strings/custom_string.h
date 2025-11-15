#ifndef MYAPI_H
#define MYAPI_H

// Export/import macro
// CUSTOM_STRING_API is a macro, you can named it
#ifdef MYAPI_EXPORTS
#   define CUSTOM_STRING_API __declspec(dllexport)
#else
#   define CUSTOM_STRING_API __declspec(dllimport)
#endif

#include <Windows.h>
// Your function
CUSTOM_STRING_API VOID cstrcpy(WCHAR *desc, const WCHAR *src, DWORD desc_size, DWORD src_size);

#endif
