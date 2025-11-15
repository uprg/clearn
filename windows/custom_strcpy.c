/* 
cl /TC /showIncludes /O2 /Wall /Zi /DEBUG server.c /link ws2_32.lib 

cl /TC /O2 /Wall /Zi /DEBUG /fsanitize=address,undefined server.c /link ws2_32.lib

cl /TC /O2 /Wall /Zi /DEBUG /fsanitize=address,undefined windows\custom_strlen.c
*/




/*
_WIN32_WINNT:
    Tells the compiler “the minimum Windows version this program supports.”	
    0x0A00 → Windows 10/11

NTDDI_VERSION:
    Tells the SDK “the specific build (and service pack) of Windows you're targeting.”	
    NTDDI_WIN10, NTDDI_WIN10_RS4, NTDDI_WIN11

*/
#define _WIN32_WINNT 0x0A00
#define NTDDI_VERSION NTDDI_WIN10


#include <stdio.h>

void cstrcpy(char *desc, char *src){
    while (*src != '\0'){
        *desc = *src;
        src++;
        desc++;
    }

    *desc = '\0';
}

int wmain(int argc, wchar_t *argv[])
{
    char src[] = "what";
    char desc[5] = {0};
    cstrcpy(desc, src);
    printf("data: %s", desc);
    return 0;
}