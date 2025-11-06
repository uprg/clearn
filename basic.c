/*
 * afl-gcc, memcpy, elf file (some terms to remember)
 * */
// #include <stdio.h>

// void main(void) {
//     int myAge = 43;
//     float myAgef = 43.34;
//     char myAgec = '4';
//     char myAgesc[] = "43";
//     int *ptr = &myAge;  // pointer to myAge

//     printf("myAge value: %d\n", myAge);      // prints 43
//     printf("myAge address: %p\n", (void*)&myAge); // prints address
//     printf("ptr value (address): %p\n", (void*)ptr); // same address as &myAge
//     printf("value at ptr: %d\n", *ptr);     // prints 43 (dereference)
//     printf("%zu\n", sizeof(myAge));
//     printf("%zu\n", sizeof(myAgef));
//     printf("%zu\n", sizeof(myAgec));
//     printf("%zu\n", sizeof(myAgesc));

//     // return 0;
// }


// #include <stdio.h>

// int main(void) {
//     // Step 1: Declare a FILE pointer
//     FILE *filePtr;

//     // Step 2: Open a file in write mode ("w")
//     filePtr = fopen("example.txt", "w");
//     if (filePtr == NULL) {
//         // fopen returns NULL if the file could not be opened
//         printf("Error opening file!\n");
//         return 1;  // Exit with error
//     }

//     // Step 3: Write something to the file
//     fprintf(filePtr, "Hello, this is a file created using C!\n");
//     fprintf(filePtr, "You can write multiple lines.\n");

//     // Step 4: Close the file
//     fclose(filePtr);

//     printf("myAge address: %p\n", (void*)&filePtr);
//     printf("File written successfully!\n");
//     return 0;
// }




// #include <stdio.h>
// // #include <stdlib.h>

// int main(void) {
//     FILE *srcFile, *dstFile;
//     char buffer[4096];   
//     size_t bytesRead;

//     srcFile = fopen("input.jfif", "rb");
//     if (srcFile == NULL) {
//         printf("Error opening source file!\n");
//         return 1;
//     }

//     dstFile = fopen("output.jpg", "wb");
//     if (dstFile == NULL) {
//         printf("Error opening destination file!\n");
//         fclose(srcFile);
//         return 1;
//     }

//     while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcFile)) > 0) {
//         fwrite(buffer, 1, bytesRead, dstFile);
//     }

//     fclose(srcFile);
//     fclose(dstFile);

//     printf("Image copied successfully!\n");
//     return 0;
// }





// #include <stdio.h>
// #include <stdlib.h>

// int main(void) {
//     struct salves
//     {
//         int age;
//         char name[6];
//         float salary;
//     };


//     struct salves *ptr = malloc(sizeof(struct salves));

//     (*ptr).age = 35;
//     snprintf((*ptr).name, sizeof((*ptr).name), "Steve");
//     (*ptr).salary = 100.00;


//     printf("Age: %d\n", ptr->age);
//     printf("Name: %s\n", ptr->name);
//     printf("Salary: %.2f\n", ptr->salary);


//     ptr->age = 40;
//     snprintf(ptr->name, sizeof(ptr->name), "Bill");
//     ptr->salary = 1000.00;

//     printf("Age: %d\n", ptr->age);
//     printf("Name: %s\n", ptr->name);
//     printf("Salary: %.2f\n", ptr->salary);

//     free(ptr);
    
//     return 0;
// }



// #include <stdio.h>

// int main() {
//     int a = 1;
//     int b = 2;
//     char buffer[50];

//     snprintf(buffer, sizeof(buffer), "{ \"a\": %d, \"b\": %d }", a, b); // just puts the json data into buffer, can cut of json data if buffer is small

//     printf("%s\n", buffer);  // prints: { "a": 1, "b": 2 }

//     return 0;
// }



#include <stdio.h>
#include <stdlib.h>

/*
It can be int main(void) or size_t main(void) as int or size_t are just return types of the func.

As size_t is an unsigned int datatype according to platform(x86 or x64).

So it can be 4bytes in 32-bit or 8bytes in 64-bit.

and stores size value in int, like 4 for 4 bytes and so on.

In the main func case its kind of an hack, as it can take an integer.
*/

size_t main(void) {
    struct Humans
    {
        size_t age;
        char name[6];
        float salary;
    };


    struct Humans *ptr = malloc(sizeof(struct Humans));

    (*ptr).age = 35;
    snprintf((*ptr).name, sizeof((*ptr).name), "Steve");
    (*ptr).salary = 100.00;


    printf("Age: %d\n", ptr->age);
    printf("Name: %s\n", ptr->name);
    printf("Salary: %.2f\n", ptr->salary);


    ptr->age = 40;
    snprintf(ptr->name, sizeof(ptr->name), "Bill");
    ptr->salary = 1000.00;

    printf("Age: %d\n", ptr->age);
    printf("Name: %s\n", ptr->name);
    printf("Salary: %.2f\n", ptr->salary);

    free(ptr);
    
    return 0;
}
