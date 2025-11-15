/**
 * Build DLL
 * cl /LD windows\custom_strings\custom_string.c /Fe:windows\custom_strings\custom_string.dll
 * 
 * Compile
 * cl windows\custom_strings\test_custom_string_dll.c /Fe:windows\custom_strings\test_custom_string.exe /I windows\custom_strings /link windows\custom_strings\custom_string.lib
 * 
 * Run
 * windows\custom_strings\test_custom_string.exe
 * 
 * DLL and EXE must be in same folder, side by side
 * Include has header files
 */


#include <Windows.h>
#include <wchar.h>
#include "custom_string.h"


int wmain(int argc, wchar_t *argv[])
{
    const WCHAR src[] = L"so this string is copied by my custom strcpy!";
    WCHAR desc[46];
    DWORD desc_size = sizeof(desc) / sizeof(WCHAR);
    DWORD src_size = sizeof(src) / sizeof(WCHAR);
    cstrcpy(desc, src, desc_size, src_size);
    wprintf(L"data: %ls", desc);
    return 0;
}