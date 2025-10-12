/*
 * nectcat command: echo "Your data here" | nc 0.0.0.0 8080
 * gcc webserver.c -fsanitize=address,undefined -g
 * */

#include <sys/socket.h> 
#include <netinet/in.h> /* struct sockaddr_in */
#include <stdio.h>
#include <arpa/inet.h> /* htons */
#include <unistd.h> /* close */
#include <stdlib.h> /* malloc */

int  main(){
  int sfd; /* file desc for scoket */
  struct sockaddr_in addr; /*ipv4 family struct */

  addr.sin_family = AF_INET; /*use ipv4 address family */
  /* port, htons is used for system compatiablity like little-endian and big-endian */
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = INADDR_ANY; /* use any ipv4 address, which is 0.0.0.0 */

  /* create socket 
   * @AF_INET: with ip address family, 
   * @SOCK_STREAM: type of stream (duplex), 
   * @0: which protocol, some have multiple protocol so we use the default one (0) 
   * */

  sfd = socket(AF_INET, SOCK_STREAM, 0);

  /* bind scoket with address and port 
   * 
   * @sfd: socket file desc
   * 
   * @(struct sockaddr *)&addr: type casted sockaddr_in struct to sockaddr
   * as bind implement generic struct sockaddr but we need sockaddr_in for ipv4 address family and port
   * addr a struct member which is not a pointer, so we need address, so we get the value and type at that address
   * then we change the value type
   *
   * @sizeof(addr): size of the struct addr
   * */

  bind(sfd, (struct sockaddr *)&addr, sizeof(addr));



  /* listen for connections
   *
   * @sfd: server file desc
   *
   * @0: len of con queue
   * */
  listen(sfd, 0);
  printf("Server started listening on http://0.0.0.0:8080\n");

  /* infinte loop to keep the server running and accepting new connections */
  while (1){

  /* client structs and vars */
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  char *buf = malloc(1024);
  int csfd;

  /* accepting conn
   * same as bind except the last param is a pointer
   *
   * @sfd: socket file desc
   *
   * @(struct sockaddr *)&client_addr: type casted sockaddr_in struct to sockaddr for client
   *
   * @&client_addr_size: address of size of client_addr or socklen_t type pointer that points to size of client_addr, which struct of sockaddr_in 
   * */
  csfd = accept(sfd, (struct sockaddr *)&client_addr, &client_addr_size);

  /* recv_size stores the size of message received */
  int recv_size;

  /* recv data from client
   * 
   * @csfd: client file desc
   * 
   * @buf: buffer of 1024 bytes
   *
   * @1023: buffer size 1024 - 1, so that we can assign null terminator '\0', as data got from recv doesn't end string the way c wanted
   *
   * @0: zero means no flags
   * */
  recv_size = recv(csfd, buf, 1023, 0);
  buf[recv_size] = '\0';
  
  /* print the buffer data got from client */
  printf("data from client: %s\n", buf);

  /* close client file desc */
  close(csfd);
  }

  return 0;
}
