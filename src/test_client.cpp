#include <iostream>
#include <stdlib.h>  // for strol
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
//network
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 4444
#define HOSTIP "169.234.30.70"
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
	pid_t childpid;
  int nbytes;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	bzero(buffer, sizeof(buffer));

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

	if((childpid=fork()) ==1){ //Make a child process
      perror("fork() error\n");
      exit(0);
  }
  else if(childpid == 0) { //Part of child process.
      while(1){        //Send messgae to Server using write system call. If insert exit, exit the process.
          fgets(buffer,sizeof(buffer),stdin);
          nbytes = strlen(buffer);
          write(clientSocket,buffer,511);
          if((strncmp,"exit",4) == 0){
              puts("Good Bye.");
							kill( getppid(), SIGINT);
              exit(0);
          }
      }
      exit(0);
  }
  else if(childpid>0){ //Part of parent process.
      while(1){    //Print the message which Server send to Client. If insert exit, exit the process.
					bzero(buffer, sizeof(buffer));
					if(nbytes = read(clientSocket,buffer,511) <0){
              perror("read() error\n");
              exit(0);
          }
          printf("Server : %s",buffer);
          if(strncmp(buffer,"exit",4) == 0)
              exit(0);

					if(strncmp(buffer, "stop", 4) == 0){
						write(clientSocket, buffer, 511);
						kill(childpid, SIGINT);
						exit(0);
					}

					if(strncmp(buffer, "checking", 8) == 0){
								while(tmp->request == false);
								char powerErr[10];
								sprintf(powerErr, "%f\n", tmp->powErr);
								bzero(buffer, sizeof(buffer));
								strcpy(buffer, "powerErr : ");
								strcat(buffer, powerErr);
								tmp->freq = tmp->powErr + 10;
								if(sem_post(ack_sem) == -1) //end ipc
									error("sem_post : buffer_count_sen");

								//send to server
								write(clientSocket, buffer, 511);
					}
      }
  }

	shmdt(tmp); // detach from shared memory
	return 0;
}
