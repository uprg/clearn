#include <stdio.h>
#include <string.h>

void parse_line(char *token, char *method, char *route, char *http_version){
    const char *innererdelimiter = " ";
    char *token1 = strtok(token, innererdelimiter);
    
    while (token1 != NULL){
        if (strcmp(token1, "POST") == 0 || strcmp(token1, "GET") == 0 || strcmp(token1, "PUT") == 0 || strcmp(token1, "DELETE") == 0){
            strcpy(method, token1);
        }
        
        if (strcmp(token1, "/getData") == 0){
            strcpy(route, token1);
        }
        
        if (strcmp(token1, "HTTP/1.1") == 0){
            strcpy(http_version, token1);
        }
        
        token1 = strtok(NULL, innererdelimiter);
    }
}

int main()
{
    char method[7] = {0};
    char route[9] = {0};
    char http_version[9] = {0};
    
    char request[] = "POST /getData HTTP/1.1\r\nContent-Type: application/json\r\n\r\n{\"message\": \"done\"}";
    const char *outerdelimiter = "\r\n";
    
    char *token = strtok(request, outerdelimiter);
    
    while (token != NULL){
        parse_line(token, method, route, http_version);
        token = strtok(NULL, outerdelimiter);
    }
    
    printf("Method: %s\n", method);
    printf("Route: %s\n", route);
    printf("HTTP Version: %s\n", http_version);
    
    return 0;
}


// #include <stdio.h>
// #include <string.h>

// void parse_line(char *token, char *method, char *route, char *http_version){
//     const char *innererdelimiter = " ";
//     char *token1 = strtok(token, innererdelimiter);
    
//     while (token1 != NULL){
        
//         if (*token1 == 'G' || *token1 == 'g' || *token1 == 'P' || *token1 == 'p' || *token1 == 'D' || *token1 == 'd'){
//             strcpy(method, token1);
//         }
                
//         if (*token1 == '/'){
//             strcpy(route, token1);
//         }
                
//         if (*token1 == 'H' || *token1 == 'h'){
//             strcpy(http_version, token1);
//         }
        
//         token1 = strtok(NULL, innererdelimiter);
//     }
// }

// int main()
// {
//     char method[5] = {0};
//     char route[9] = {0};
//     char http_version[9] = {0};
    
//     char request[] = "POST /getData HTTP/1.1\r\nContent-Type: application/json\r\n\r\n{\"message\": \"done\"}";
//     const char *outerdelimiter = "\r\n";
    
//     char *token = strtok(request, outerdelimiter);
    
//     while (token != NULL){
//         parse_line(token, method, route, http_version);
//         token = strtok(NULL, outerdelimiter);
//     }
    
//     printf("Method: %s\n", method);
//     printf("Route: %s\n", route);
//     printf("HTTP Version: %s\n", http_version);
    
//     return 0;
// }