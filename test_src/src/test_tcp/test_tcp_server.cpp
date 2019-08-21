#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 4567
#define PACKET_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[]){
  int server_sock;
  int client_sock;

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  socklen_t client_addr_size;

  server_sock = socket(PF_INET, SOCK_STREAM, 0);
  if(server_sock == -1){
    error_handling("socket() error");
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(PORT);

  if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
    error_handling("bind() error");
  }

  if(listen(server_sock, 5) == -1){
    error_handling("listen() error");
  }

  client_addr_size = sizeof(client_addr);
  client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);

  if(client_sock==-1){
    error_handling("accept() error");
  }

  char message[] = "Hello World (Server)";
  char cBuffer[PACKET_SIZE] = {};

  write(client_sock, message, sizeof(message));
  read(client_sock, cBuffer, sizeof(cBuffer));

  printf("Message from client: %s\n", cBuffer);

  close(client_sock);
  close(server_sock);
  return 0;
}

void error_handling(char* message){
  fputs(message, stderr);
  fputc('\n',stderr);
  exit(1);
}
