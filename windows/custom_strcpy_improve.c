/**
 * cl /TC /O2 /Wall /Zi /DEBUG /fsanitize=address,undefined windows\custom_strcpy_improve.c
 */
#include <stdio.h>

/**
 * func returns a desc pointer now
 */
void *cstrcpyi(char *desc, const char *src){
    /**
     * copying pointer so that our desc pointer does not increment
     * and point to some other address rather than the first position
     */
    char *ptr = desc;

    /**
     * while loop break if the '\0' is encoutered as it is treated as false
     * values are copied from one to one position
     * then at last the post increment (++) happens
     */
    while (*src) *ptr++ = *src++;

    *ptr = 0; // assigning 0 means doing '\0'

    return desc; // reurning original pointer whose position is unchanged
}




int main(void){
    const char src[] = "so this string is copied by my custom strcpy!";
    char desc[46];

    /**
     * char array decays into a pointer of type char inside a func call
     */
    char *output = cstrcpyi(desc, src);

    /**
     * the pointer ouput start from start and goes till end
     * printing everything
     * 
     * if the output was an array of char, it would decay into a pointer
     * pointin the first char and print everything til end
     */
    printf("Copied string: %s", output);

}