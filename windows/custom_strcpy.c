/**
 * Compile command:
 * cl /TC /showIncludes /O2 /Wall /Zi /DEBUG server.c /link ws2_32.lib
 * cl /TC /O2 /Wall /Zi /DEBUG /fsanitize=address,undefined server.c /link ws2_32.lib
 * cl /TC /O2 /Wall /Zi /DEBUG /fsanitize=address,undefined windows\custom_strcpy.c
 * 
 * these gets imports by default, manually adding can result in linker issues
 * for some runtimes(lib and dll) like ASan used by fsanitize
 * (ucrt.lib, vcruntime.lib, msvcrt.lib (in some configs))
 * 
 * below libs are for /fsanitize=address,undefined
 * cl /TC /O2 /Wall /Zi /DEBUG /fsanitize=address,undefined windows\custom_strcpy.c
 * /link clang_rt.asan_dynamic-x86_64.lib clang_rt.asan_dynamic_runtime_thunk-x86_64.lib
 * 
 * cl /TC /O2 /Wall /Zi /DEBUG /fsanitize=address,undefined windows\custom_strcpy.c
 */



/**
 * _WIN32_WINNT:
 * Tells the compiler “the minimum Windows version this program supports.”
 * 0x0A00 → Windows 10/11
 * 
 * NTDDI_VERSION:
 * Tells the SDK “the specific build (and service pack) of Windows you're targeting.”
 * NTDDI_WIN10, NTDDI_WIN10_RS4, NTDDI_WIN11
 */

#define _WIN32_WINNT 0x0A00
#define NTDDI_VERSION NTDDI_WIN10

/**
 * wchar_t is wide charater, its in standard c lib, it is not from windows
 * #include <stdio.h> has printf
 */

/**
 * NOTE: trying to have every datatype of 32 bit, some might not be but most are 
 */

/**
 * Windows.h has lot of types
 * DWORD (typedef unsigned long DWORD) 4 bytes (32 bit)
 * WCHAR (typedef wchar_t WCHAR) 2 bytes (16 bit)
 * VOID (typedef void VOID)
 */
#include <Windows.h>

/**
 * wprintf (printf for wide charaters)
 */
#include <wchar.h>


/**
 * TODO: things of solutions for buffer overflow cases
 * desc length is less than src length, that means we will write mem address that do not belong to desc
 * desc is garbage initialize, so no '\0' (null terminator) for string, loop can go forever
 * src can also be garbage initialize
 * 
 * kinda solution:
 * get length of desc and src, desc must have grabage values and a null terminator at the right index
 * no zero initialize as all elements of the char array will be null terminator
 * WCHAR arr[5] = {0}; means {L'\0', L'\0', L'\0', L'\0', L'\0'}
 * L is for wide char
 * 
 * C has some limitations, src must be a known length pointer to char
 * other wise func will fail
 * 
 * if arr[5] is zero initialize or garbage initialize the array len is always 5
 */


/**
 * cstrcpy for linux:
 * void cstrcpy(char *desc, const char *src, size_t desc_size){
 *  while (*src != '\0'){
 *      *desc = *src;
 *      src++;
 *      desc++;
 *  }
 *  *desc = '\0';
 * }
 * 
 * sizeof returns num of bytes not the length of variable
 * size_t (typedef unsigned long size_t)
 */

VOID cstrcpy(WCHAR *desc, const WCHAR *src, DWORD desc_size, DWORD src_size){

    if (desc_size < src_size){
        return;
    }

    while (*src != L'\0'){
        *desc = *src;
        src++;
        desc++;
    }

    *desc = L'\0';
}

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