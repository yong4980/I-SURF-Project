#include "mars_tcp.h"
Tcp::Tcp(){
  checkStr = new char[1024];
  buffer = new char[1024];
  bzero(checkStr, sizeof(checkStr));
  bzero(buffer, sizeof(buffer));
  connectedSocket = new int[MAXCLIENT];
  for(int i=0; i<MAXCLIENT; i++){
    connectedSocket[i] = 0;
  }

  key = ftok("shmList", 100); //Ftok to generate unique key
  shmid = shmget(key, 1024, 0666|IPC_CREAT); //Shmget(make shm) returns an identifier in shmid
  connectedSocket = (int*) shmat(shmid, (void*)0, 0); //Shmat to attach to shared memory
}

Tcp::~Tcp(){
  shmdt(connectedSocket);
  delete[] connectedSocket;
}

int Tcp::BuildServerTCP(){
  listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
  //Check creating socket
  if(listeningSocket < 0){
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
  check = bind(listeningSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  if(check<0){
    printf("[-]Error in binding.\n");
    exit(1);
  }
  printf("[+]Bind to port %d\n", PORT);

  //Listening
  if(listen(listeningSocket, 10)==0){
    printf("Listening....\n");
  }
  else{
    printf("[-]Error in binding.\n");
  }

  return listeningSocket;
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

int Tcp::ListeningClient(){
  int newSocket = accept(listeningSocket, (struct sockaddr*)&newClientAddr, &addr_size);
  int tmpIndex = FindEmptySocket();
  connectedSocket[tmpIndex] = newSocket;
  if(newSocket < 0){
    exit(1);
  }
  printf("[+]Connection accepted from %s:%d\n", inet_ntoa(newClientAddr.sin_addr), ntohs(newClientAddr.sin_port));

  return newSocket;
}

char* Tcp::ReadMsg(int socket){
  bzero(buffer, sizeof(buffer));
  if(nbytes = read(socket,buffer,511) <0){
      perror("read() error\n");
      exit(0);
  }
  return buffer;
}

void Tcp::WriteMsg(int socket, char* msg){
  write(socket, msg, 511);
}

int Tcp::FindEmptySocket(){
  for(int i=0; i<MAXCLIENT; i++){
    if(connectedSocket[i] == 0){
      return i;
    }
  }
  return -1; //Full
}
