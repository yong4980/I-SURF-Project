#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

void test(char* buffer, struct sockaddr_in newAddr, int newSocket){
  int i;
  if(strcmp(buffer, "testing1") == 0){
    send(newSocket, buffer, strlen(buffer), 0);
    for(i=0; i<10; i++){
      printf("Client(%d) complete1\n", ntohs(newAddr.sin_port));
      sleep(1);
    }
  }
  else if(strcmp(buffer, "testing2") == 0){
    send(newSocket, buffer, strlen(buffer), 0);
    for(i=0; i<10; i++){
      printf("Clinet(%d) complete2\n", ntohs(newAddr.sin_port));
      sleep(1);
    }
  }
  else{
    send(newSocket, buffer, strlen(buffer), 0);
  }
}

int main(){

  //testing
  int boardNum = 0;

  int sockfd, ret;
  struct sockaddr_in serverAddr;

  int newSocket;
  struct sockaddr_in newAddr;

  socklen_t addr_size;

  char buffer[1024];
  pid_t childpid;

  bzero(buffer, sizeof(buffer));

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Server Socket is created.\n");

  memset(&serverAddr, '\0', sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr("169.234.31.35");

  ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  if(ret < 0){
    printf("[-]Error in binding.\n");
    exit(1);
  }
  printf("[+]Bind to port %d\n", PORT);

  if(listen(sockfd, 10)==0){
    printf("Listening....\n");
  }
  else{
    printf("[-]Error in binding.\n");
  }

  while(1){
    newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
    if(newSocket < 0){
      exit(1);
    }
    printf("[+]Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

    if(childpid = fork() == 0){
      close(sockfd);

      while(1){
        recv(newSocket, buffer, 1024, 0);
        if(strcmp(buffer, ":exit") == 0){
          printf("[-]Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
          break;
        }
        else{
          printf("Client(%d): %s\n", ntohs(newAddr.sin_port), buffer);
          //send(newSocket, buffer, strlen(buffer), 0);
          test(buffer, newAddr, newSocket);
          bzero(buffer, sizeof(buffer));
        }
      }
    }
  }

  close(newSocket);

  return 0;
}