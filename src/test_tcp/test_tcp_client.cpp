#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 4567
#define PACKET_SIZE 1024
#define SERVER_IP "169.234.56.234"
void error_handling(char* message);

int main(int argc, char* argv[]){
  int sock;
  struct sockaddr_in server_addr;

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1){
    error_handling("socket() error");
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  server_addr.sin_port = htons(PORT);

  if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
    error_handling("connect() error");
  }

  char message[] = "Client Message";
  char cBuffer[PACKET_SIZE] = {};

  read(sock, cBuffer, sizeof(cBuffer));
  write(sock, message, sizeof(message));

  printf("Message from server: %s\n", cBuffer);

  close(sock);
  return 0;
}

void error_handling(char* message){
  fputs(message, stderr);
  fputc('\n',stderr);
  exit(1);
}
