#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>
#include <fcntl.h>
#include <semaphore.h>
#define SEM_REQUEST_SIGNAL_NAME "/sem-req"
#define SEM_ACK_SIGNAL_NAME "/sem-ack"
void UIMars(int *inputKey);

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

int main(int argc, char* argv[])
{
	int inputKey;
	UIMars(&inputKey);

	key_t key = ftok("shmfile", inputKey); // ftok to generate unique key
	int shmid = shmget(key, 1024, 0666|IPC_CREAT); // shmget returns an identifier in shmid
	fuzzy_data *client = (fuzzy_data*) shmat(shmid, (void*)0, 0); // shmat to attach to shared memory
	sem_t *req_sem, *ack_sem;
	// Initialize semaphore with 0
	if((req_sem = sem_open(SEM_REQUEST_SIGNAL_NAME, 0, 0, 0)) == SEM_FAILED)
		printf("sem_open failed : req_sem");
	// counting semaphore, indicating the number of strings to be printed. Initial value=0
	if((ack_sem = sem_open(SEM_ACK_SIGNAL_NAME, 0, 0, 0)) == SEM_FAILED)
		printf("sem_open failed : ack_sem");

	double powerErr;

	//scanf powErr
	printf("Power Err -> ");
	scanf("%lf", &powerErr);
	client->powErr = powerErr;

	// Send request
	client->request = true;

	// get an ack
	printf("Waiting for fuzzy");
	while(sem_trywait (ack_sem) == -1)
		printf(".");
	printf("Stop waiting\n");

	client->request = false;

	printf("\nresult\nfreq:%d\nEnd MARS\n", client->freq);

	return 0;
}

void UIMars(int *inputKey){
	printf("Please input Shared memory key --> ");
	scanf("%d", inputKey);
}
