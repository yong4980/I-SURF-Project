#include "mars_tcp.h"
#include <signal.h>
void ClientFunction(int clientSocketNum, char* buffer, Tcp* clientTcp, pid_t childpid);
void GetString(char* str);

int main(void){
  char* buffer = new char[1024];
  int clientSocket;
  pid_t childpid;

  //Connect TCP
  Tcp clientTcp;
  clientSocket = clientTcp.BuildClientTCP();

  if((childpid=fork()) == -1){
    close(clientSocket);
    perror("fork() error\n");
    exit(0);
  }
  else if(childpid == 0){//Child process, Send message to client using write system call.
    while(true){
      GetString(buffer);
      clientTcp.WriteMsg(clientSocket, buffer, 1);

      ClientFunction(clientSocket, buffer, &clientTcp, childpid); //Client functions
    }
  }
  else if(childpid > 0){//Parent Process. Receive message from server.
    while(true){
      strcpy(buffer, clientTcp.ReadMsg(clientSocket, 1));
      printf("Server : %s", buffer);

      ClientFunction(clientSocket, buffer, &clientTcp, childpid);
    }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////
/*If you want to make function which is executed in client, modify this function*/
//////////////////////////////////////////////////////////////////////////////////
void ClientFunction(int clientSocket, char* buffer, Tcp* clientTcp, pid_t childpid){
  if(strncmp(buffer, "exit", 4) == 0){
    delete[] buffer;
    exit(0);
  }
  else if(strncmp(buffer, "stopRecv", 8) == 0){
    printf("\n");
    delete[] buffer;
    exit(0);
  }
  else if(strncmp(buffer, "stop", 4) == 0){
    printf("\n");
    strcpy(buffer, "stopRecv");
    clientTcp->WriteMsg(clientSocket, buffer, 1);
    kill(childpid, SIGINT);
    delete[] buffer;
    exit(0);
  }
}
//////////////////////////////////////////////////////////////////////////////////

void GetString(char* str){
  bzero(str, sizeof(str));
  fflush(stdin);
  fgets(str,sizeof(str),stdin);
}
