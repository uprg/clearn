#define MYAPI_EXPORTS   // IMPORTANT
#include "custom_string.h"

CUSTOM_STRING_API VOID cstrcpy(WCHAR *desc, const WCHAR *src, DWORD desc_size, DWORD src_size){

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