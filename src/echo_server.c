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
  pid_t childpid, childpid2;

  int socketNum = 0, nbytes, checkNum;
  char checkStr[1024];

  bzero(buffer, sizeof(buffer));
  bzero(checkStr, sizeof(checkStr));

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Server Socket is created.\n");

  memset(&serverAddr, '\0', sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr("169.234.11.116");

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
    socketNum += 1;

    if((childpid=fork())== -1){close(sockfd);perror("fork() error");exit(0);}
    else if (childpid == 0){ //자식프로세스이다.
        while(1){
            fflush(stdin);
            printf("socket Num : ");
            fgets(checkStr, sizeof(checkStr), stdin);
            checkNum = atoi(checkStr);
            if(checkNum == socketNum){
              bzero(checkStr, sizeof(checkStr));
              checkNum = 0;

              printf("input : ");
              fflush(stdin);
              fgets(buffer,sizeof(buffer),stdin);
              nbytes = strlen(buffer);
              write(newSocket,buffer,511);
              if((strncmp,"exit",4) == 0){
                  puts("Good Bye.");
                  exit(0);
              }
            }
        }
        exit(0);
    }

    if((childpid2=fork())== -1){close(sockfd);perror("fork() error");exit(0);}
    else if(childpid2 == 0){ //또 다른 자식프로세스이다.
        while(1){
            if((nbytes = read(newSocket,buffer,511)) <0){
                perror("read() error\n");
                exit(0);
            }
            printf("%s",buffer);
            if(strncmp(buffer,"exit",4) == 0)
                exit(0);
        } //부모프로세스는 client가 소켓에 보낸 문자열을 read로 읽어 저장한다.
    }     //역시 exit 가 날라

  }
  close(newSocket);

  return 0;
}
