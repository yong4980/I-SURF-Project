#include "mars_tcp.h"
Tcp::Tcp(){
  buffer = new char[1024];
  bzero(buffer, sizeof(buffer));
  connectedSocket = new int[MAXSOCKET];
  for(int i=0; i<MAXSOCKET; i++){
    connectedSocket[i] = 0;
  }
}

Tcp::~Tcp(){
}

int Tcp::BuildServerTCP(){
  //shared memory
  key = ftok("shmList", 100); //Ftok to generate unique key
  shmid = shmget(key, 1024, 0666|IPC_CREAT); //Shmget(make shm) returns an identifier in shmid
  connectedSocket = (int*) shmat(shmid, (void*)0, 0); //Shmat to attach to shared memory

  connectedSocket[0] = socket(AF_INET, SOCK_STREAM, 0); //listeningSocket
  //Check creating socket
  if(connectedSocket[0] < 0){
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Server Socket is created.\n");

  //Set server info
  memset(&serverAddr, '\0', sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons( PORT );
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  //Bind
  check = bind(connectedSocket[0], (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  if(check < 0){
    printf("[-]Error in binding.\n");
    exit(1);
  }
  printf("[+]Bind\n");

  //Listening
  if(listen(connectedSocket[0], 10)==0){
    printf("Listening....\n");
  }
  else{
    printf("[-]Error in binding.\n");
  }

  return 0;
}

int Tcp::BuildClientTCP(){
  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  //Check creating socket
  if(clientSocket < 0){
		printf("[-]Error in Connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

  //Set server info
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(HOSTIP);

  //Connect
  check = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(check < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

  return clientSocket;
}

void Tcp::ListeningClient(int newSocketNum){
  int newSocket = accept(connectedSocket[0], (struct sockaddr*)&newClientAddr, &addr_size);
  connectedSocket[newSocketNum] = newSocket;
  if(newSocket < 0){
    exit(1);
  }
  printf("[+]Connection accepted from %s:%d\n", inet_ntoa(newClientAddr.sin_addr), ntohs(newClientAddr.sin_port));
}

char* Tcp::ReadMsg(int socketNum){
  int nbytes;
  bzero(buffer, sizeof(buffer));
  if((nbytes = read(connectedSocket[socketNum],buffer,1023)) < 0){
      perror("read() error\n");
      exit(0);
  }
  return buffer;
}

char* Tcp::ReadMsg(int socket, int client){
  int nbytes;
  bzero(buffer, sizeof(buffer));
  if((nbytes = read(socket,buffer,1023)) < 0){
      perror("read() error\n");
      exit(0);
  }
  return buffer;
}

void Tcp::WriteMsg(int socketNum, char* msg){
  write(connectedSocket[socketNum], msg, 1023);
}

void Tcp::WriteMsg(int socket, char* msg, int client){
  write(socket, msg, 1023);
}

void Tcp::WriteMsg(char* socketNumStr, char* msg){
  int socketNum = atoi(socketNumStr);
  if(connectedSocket[socketNum] == 0){
    printf("That socket is not connected\n");
  }
  else{
    write(connectedSocket[socketNum], msg, 1023);
  }

}

int Tcp::FindEmptySocket(){
  for(int i=1; i<MAXSOCKET; i++){
    if(connectedSocket[i] == 0){
      return i;
    }
  }
  return -1; //Full
}

void Tcp::CheckConnectedSocket(){
  int cnt = 0;
  printf("Connected Socket num : ");
  for(int i=1; i<MAXSOCKET; i++){
    if(connectedSocket[i] != 0){
      printf("%d ", i);
      cnt++;
    }
  }
  if(cnt==0){
    printf("None\n");
  }
  else{
    printf("\n");
  }
}

void Tcp::QuitTCP(){
  shmdt(connectedSocket);
  shmctl(shmid,IPC_RMID,NULL);
  delete[] buffer;
  printf("Complete detach shared memory\n");
}

void Tcp::QuitAll(){
  strcpy(buffer, "stop");
  for(int i=1; i<MAXSOCKET; i++){
    if(connectedSocket[i] != 0){
      WriteMsg(i, buffer);
      connectedSocket[i] = 0;
    }
  }
  connectedSocket[0] = 0;
  printf("Disconnect all clients\n");
}

void Tcp::close(int socketNum){
  close(connectedSocket[socketNum]);
}

void Tcp::DisconnectSocket(int socketNum){
  connectedSocket[socketNum] = 0;
}
