#include <iostream>
#include <stdlib.h>  // for strol
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>
#include <fcntl.h>
#include <semaphore.h>
//network
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 4444
#define HOSTIP "169.234.10.114"
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

void error (string msg)
{

}

int main(int argc, char* argv[])
{
	//network 
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in Connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(HOSTIP);

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	//ipc 
	key_t key = ftok("shmfile", 65); // ftok to generate unique key
	
	int shmid = shmget(key, 1024, 0666|IPC_CREAT); // shmget returns an identifier in shmid

	fuzzy_data *tmp = (fuzzy_data*) shmat(shmid, (void*)0, 0); // shmat to attach to shared memory
	sem_t *req_sem, *ack_sem;
	if((req_sem = sem_open(SEM_REQUEST_SIGNAL_NAME, O_CREAT, 0600, 0)) == SEM_FAILED)
		error ("sem_open");
	if((ack_sem = sem_open(SEM_ACK_SIGNAL_NAME, O_CREAT, 0600, 0)) == SEM_FAILED)
		error ("sem_open");

	//int count = atoi(argv[1]);

/*	//ipc
	while(1) {
		//wait for a request
		while(tmp->request == false);
		
		//powerErr = tmp->powErr; //set test_mars
		//int frequencyVal = fuzzyDVFS(powerErr); //do not use fuzzy
		//tmp->freq = frequncyVal; //set specific value 

		//Send result to daemon 
		if (sem_post (ack_sem) == -1)
			error ("sem_post : buffer_count_sen");
	}
	*/

	//interact server
	while(1){
		bzero(buffer, sizeof(buffer));
		printf("Client : \t");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);

		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		/*testing*/	
		if(strcmp(buffer, "checking") == 0){
			// ipc
			while(tmp->request == false);
			char powerErr[10];
			sprintf(powerErr, "%f", tmp->powErr);
			bzero(buffer, sizeof(buffer));
			strcpy(buffer, "powerErr : ");
			strcat(buffer, powerErr);
			tmp->freq = tmp->powErr + 10;
			if(sem_post(ack_sem) == -1)
				error("sem_post : buffer_count_sen");
			// end ipc
			send(clientSocket, buffer, strlen(buffer), 0);
		}

		if(recv(clientSocket, buffer, 1024, 0) <0){
			printf("[-]Error in receiving data.\n");
			exit(1);
		}
		else{
			printf("server: \t%s\n", buffer);
		}
	}

	
	shmdt(tmp); // detach from shared memory
	return 0;
}
