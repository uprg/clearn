/* 
cl /TC /showIncludes /O2 /Wall /Zi /DEBUG server.c /link ws2_32.lib 

cl /TC /O2 /Wall /Zi /DEBUG /fsanitize=address,undefined server.c /link ws2_32.lib

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

#define NUM_QUEUE_CONNS 0x1


#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

/* 
 main(int argc, wchar_t *argv[]) 
 */
int wmain(void){

    WSADATA wsad;
    SOCKET server_sock, client_sock;

    struct sockaddr_in server_sock_struct, client_sock_struct;
    int client_sock_struct_len = sizeof(client_sock_struct);
    char buffer[4096] = {0};
    int buffer_batch_len = sizeof(buffer) - 1;
    int bytes = 0;
    int total_bytes = 0;

    if (WSAStartup(MAKEWORD(2,2), &wsad) != 0){
        printf("[WSA Event] WSAStartup failed\n");
        return 1;
    }

    printf("[WSA Event] WSAStartup Success\n");


    server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_sock == INVALID_SOCKET){
        /* WSAGetLastError returns int */
        printf("[WSA Event] Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("[WSA Event] Socket creation Success\n");

    server_sock_struct.sin_family = AF_INET;
    server_sock_struct.sin_port = htons(8080);
    inet_pton(AF_INET, "0.0.0.0", &server_sock_struct.sin_addr);
    
    if (bind(server_sock, (struct sockaddr *)&server_sock_struct, sizeof(server_sock_struct)) == SOCKET_ERROR){
        printf("[WSA Event] Bind on ip 0.0.0.0 and port 8080 failed: %d\n", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        return 1;
    }


    printf("[WSA Event] Bind on ip 0.0.0.0 and port 8080 Success\n");

    if (listen(server_sock, NUM_QUEUE_CONNS) == SOCKET_ERROR){
        printf("[WSA Event] listening on port 8080 failed: %d\n", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        return 1;
    }


    printf("[WSA Event] listening socket events on 0.0.0.0:8080\n");


    while (1){
        client_sock = accept(server_sock, (struct sockaddr *)&client_sock_struct, &client_sock_struct_len);

        if (client_sock == INVALID_SOCKET){
            printf("[WSA Event] Accepting connection on 0.0.0.0:8080 failed: %d\n", WSAGetLastError());
            closesocket(server_sock);
            WSACleanup();
            return 1;
        }


        printf("[WSA Event] Connection accepted on 0.0.0.0:8080\n");


       
        bytes = recv(client_sock, buffer, buffer_batch_len, 0);

        printf("%d", bytes);

        buffer[bytes] = '\0';
        printf("[WSA Event] Data Recived: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
        closesocket(client_sock);


        // for (;;){
        //     bytes = recv(client_sock, buffer + total_bytes, buffer_batch_len, 0);

        //     printf("%d", bytes);

        //     if (bytes > 0){
        //         total_bytes += bytes;
        //     }else{
        //         buffer[total_bytes] = '\0';
        //         printf("[WSA Event] Data Recived: %s\n", buffer);
        //         closesocket(client_sock);
        //         total_bytes = 0;
        //         memset(buffer, 0, sizeof(buffer));
        //         break;
        //     }
        // }

    }

    closesocket(server_sock);
    WSACleanup();

    return 0;
}