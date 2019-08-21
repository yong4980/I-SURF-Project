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
#define HOSTIP "169.234.62.76"
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

	//int count = atoi(argv[1]);

	/*	//ipc sample
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
	// while(1){
	// 	bzero(buffer, sizeof(buffer));
	// 	printf("Client : \t");
	// 	scanf("%s", &buffer[0]);
	// 	send(clientSocket, buffer, strlen(buffer), 0);
	//
	// 	if(strcmp(buffer, ":exit") == 0){
	// 		close(clientSocket);
	// 		printf("[-]Disconnected from server.\n");
	// 		exit(1);
	// 	}
	//
	// 	/*testing*/
	// 	if(strcmp(buffer, "checking") == 0){
	// 		// ipc
	// 		while(tmp->request == false);
	// 		char powerErr[10];
	// 		sprintf(powerErr, "%f", tmp->powErr);
	// 		bzero(buffer, sizeof(buffer));
	// 		strcpy(buffer, "powerErr : ");
	// 		strcat(buffer, powerErr);
	// 		tmp->freq = tmp->powErr + 10;
	// 		if(sem_post(ack_sem) == -1)
	// 			error("sem_post : buffer_count_sen");
	// 		// end ipc
	// 		send(clientSocket, buffer, strlen(buffer), 0);
	// 	}
	//
	// 	if(recv(clientSocket, buffer, 1024, 0) <0){
	// 		printf("[-]Error in receiving data.\n");
	// 		exit(1);
	// 	}
	// 	else{
	// 		printf("server: \t%s\n", buffer);
	// 	}
	// }

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
          printf("Server : %s",buffer);
          if(strncmp(buffer,"exit",4) == 0)
              exit(0);

					if(strncmp(buffer, "checking", 8) == 0){
								while(tmp->request == false);
								char powerErr[10];
								sprintf(powerErr, "%f\n", tmp->powErr);
								bzero(buffer, sizeof(buffer));
								strcpy(buffer, "powerErr : ");
								strcat(buffer, powerErr);
								tmp->freq = tmp->powErr + 10;
								if(sem_post(ack_sem) == -1)
									error("sem_post : buffer_count_sen");
								// end ipc
								write(clientSocket, buffer, 511);
					}
      }
  }

	shmdt(tmp); // detach from shared memory
	return 0;
}
