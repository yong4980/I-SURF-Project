#include "./tcp/mars_tcp.h"
#define SEM_REQUEST_SIGNAL_NAME "/sem-req"
#define SEM_ACK_SIGNAL_NAME "/sem-ack"

// specifies the shared memory structure
struct fuzzy_data
{
	// Controller inputs
	double ipc;
	double powErr;
	// Contoller outputs
	int active_cores;
	int freq;
	//Communication flags for controller and the daemon
	bool request;
};
void ClientFunction(int clientSocketNum, char* buffer, Tcp* clientTcp,
										pid_t childpid, fuzzy_data *shmdat, sem_t *ack_sem);
void UIClient(char serverIP[], int *inputKey);
void GetString(char* str);

int main(void){
	//Get Server IP, Shared memory key
	char serverIP[15];
	int inputKey;
	UIClient(serverIP, &inputKey);

  //Tcp, Process
  char* buffer = new char[1024];
  int clientSocket;
  pid_t childpid;

	fgets(buffer, sizeof(buffer), stdin); //empty input buffer
  //IPC
	key_t key = ftok("shmfile", inputKey);
	int shmid = shmget(key, 1024, 0666|IPC_CREAT); // shmget returns an identifier in shmid
	fuzzy_data *shmdat = (fuzzy_data*) shmat(shmid, (void*)0, 0); // shmat to attach to shared memory
	sem_t *req_sem, *ack_sem;
	if((req_sem = sem_open(SEM_REQUEST_SIGNAL_NAME, O_CREAT, 0600, 0)) == SEM_FAILED)
		printf("sem_open");
	if((ack_sem = sem_open(SEM_ACK_SIGNAL_NAME, O_CREAT, 0600, 0)) == SEM_FAILED)
		printf("sem_open");

  //Connect TCP
  Tcp clientTcp;
  clientSocket = clientTcp.BuildClientTCP(serverIP);

  if((childpid=fork()) == -1){
    close(clientSocket);
    perror("fork() error\n");
    exit(0);
  }
  else if(childpid == 0){//Child process, Send message to client using write system call.
    while(true){
      GetString(buffer);
      clientTcp.WriteMsg(clientSocket, buffer, 1);

      ClientFunction(clientSocket, buffer, &clientTcp, childpid, shmdat, ack_sem); //Client functions
    }
  }
  else if(childpid > 0){//Parent Process. Receive message from server.
    while(true){
      strcpy(buffer, clientTcp.ReadMsg(clientSocket, 1));
      printf("Server : %s", buffer);

      ClientFunction(clientSocket, buffer, &clientTcp, childpid, shmdat, ack_sem);
    }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////
/*If you want to make function which is executed in client, modify this function*/
//////////////////////////////////////////////////////////////////////////////////
void ClientFunction(int clientSocket, char* buffer, Tcp* clientTcp,
										pid_t childpid, fuzzy_data *shmdat, sem_t *ack_sem){
  if(strncmp(buffer, "exit", 4) == 0){
    delete[] buffer;
    exit(0);
  }
  else if(strncmp(buffer, "stopRecv", 8) == 0){
    printf("\n");
    delete[] buffer;
		shmdt(shmdat); // detach from shared memory
    exit(0);
  }
  else if(strncmp(buffer, "stop", 4) == 0){
    printf("\n");
    strcpy(buffer, "stopRecv");
    clientTcp->WriteMsg(clientSocket, buffer, 1);
    kill(childpid, SIGINT);
    delete[] buffer;
		shmdt(shmdat); // detach from shared memory
    exit(0);
  }
	else if(strncmp(buffer, "check", 5) == 0){
		printf("Check Mars program\n");
		while(shmdat->request == false);
		char powerErr[10];
		sprintf(powerErr, "%f\n", shmdat->powErr);
		strcpy(buffer, "powerErr : ");
		strcat(buffer, powerErr);
		shmdat->freq = shmdat->powErr + 10;
		if(sem_post(ack_sem) == -1) //end ipc
			printf("sem_post : buffer_count_sen");

		//send to server
		clientTcp->WriteMsg(clientSocket, buffer, 1);
	}
}
//////////////////////////////////////////////////////////////////////////////////

void UIClient(char serverIP[], int *inputKey){
	printf("Start Client\nPlease input Server IP --> ");
	scanf("%s", serverIP);
	printf("Please input Shared memory key --> ");
	scanf("%d", inputKey);
}

void GetString(char* str){
  bzero(str, sizeof(str));
  fflush(stdin);
  fgets(str,sizeof(str),stdin);
}
