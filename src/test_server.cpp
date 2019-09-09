#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 5555
#define MAXCLIENT 30

int main(){

  //testing
  int boardNum = 0;

  int sockfd, ret;
  struct sockaddr_in serverAddr;

  int newSocket;
  struct sockaddr_in newAddr;

  socklen_t addr_size;

  char buffer[1024];
  pid_t sendpid;
  pid_t recvpid[MAXCLIENT];

  int socketNum = 0, nbytes, checkNum;
  char checkStr[1024];
  int client_socket[MAXCLIENT] = {0, };

  //set of socket descriptors
  fd_set readfds;

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
  serverAddr.sin_port = htons( PORT );
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

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
    newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size); //Part of parent process.
    if(newSocket < 0){
      exit(1);
    }
    printf("[+]Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
    socketNum += 1;
    socketNum %= 30;
    client_socket[socketNum] = newSocket;

    if(socketNum != 1){
      kill(sendpid, SIGINT);
    }

    if((sendpid=fork())== -1){
      close(sockfd);
      perror("fork() error");
      exit(0);
    }
    else if(sendpid == 0){//Part of child process.
      while(1){          //Send messgae to client using write system call.
          fflush(stdin);
          bzero(checkStr, sizeof(checkStr));
          fgets(checkStr, sizeof(checkStr), stdin);

          // if(strncmp(checkStr, "quit", 4) == 0){
          //   for(int i=0; i<MAXCLIENT; i++){
          //     write(client_socket[i], "stop\n", 511); //Need checking only connected client
          //   }
          //   kill(getppid(), SIGINT);
          //   close(newSocket);
          //   exit(0);
          // }
          //else{
            checkNum = atoi(checkStr);

            printf("send msg to %d socket : ", checkNum);
            fflush(stdin);
            fgets(buffer,sizeof(buffer),stdin);
            nbytes = strlen(buffer);
            write(client_socket[checkNum],buffer,511);
          //}
      }
      exit(0);
    }

    if((recvpid[socketNum]=fork())== -1){
      close(sockfd);
      perror("fork() error");
      exit(0);
    }
    else if (recvpid[socketNum] == 0){ //Part of child process.
      while(1){           //Print the message which Client send to Server. If insert exit, exit the process.
          if((nbytes = read(newSocket,buffer,511)) <0){
              perror("read() error\n");
              exit(0);
          }
          printf("Socket Num(%d) : %s", socketNum, buffer);

          if(strncmp(buffer,"exit",4) == 0){
            write(newSocket, buffer, 511);
            exit(0);
          }
          else if(strncmp(buffer, "stop", 4) == 0){
            exit(0);
          }
      }
      exit(0);
    }
  }
  close(newSocket);

  return 0;
}
