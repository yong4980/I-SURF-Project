#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(){
  int clientSocket, ret;
  struct sockaddr_in serverAddr;
  char buffer[1024];
  pid_t childpid;
  int nbytes;

  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  bzero(buffer, sizeof(buffer));

  if(clientSocket < 0){
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Client Socket is created.\n");

  memset(&serverAddr, '\0', sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr("169.234.11.116");

  ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  if(ret < 0){
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Connected to Server.\n");

  if((childpid=fork()) ==1){ //다중 프로세스를 위한 fork함수. 자식 프로세스 생성
      perror("fork() error\n");
      exit(0);
  }
  else if(childpid == 0) { //자식 프로세스 부분. stdin로 사용자가 입력한 문자를 buf에 저장하여 소켓에다
      while(1){        //write 시스템콜을 이용해 server에게 문자를 보낸다. exit 입력시 종료
          fgets(buffer,sizeof(buffer),stdin);
          nbytes = strlen(buffer);
          write(clientSocket,buffer,511);
          if((strncmp,"exit",4) == 0){
              puts("Good Bye.");
              exit(0);
          }
      }
      exit(0);

  }
  else if(childpid>0){ //부모프로세스. server가 보낸 문자열을 받아 출력한다.
      while(1){    //역시 exit 를 받을시 종료
          if(nbytes = read(clientSocket,buffer,511) <0){
              perror("read() error\n");
              exit(0);
          }
          printf("%s",buffer);
          if(strncmp(buffer,"exit",4) == 0)
              exit(0);
          }
  }
  return 0;
}
