#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>
#include <fcntl.h>
#include <semaphore.h>

using namespace std;

#define SEM_REQUEST_SIGNAL_NAME "/sem-req"
#define SEM_ACK_SIGNAL_NAME "/sem-ack"

// specifies the shared memory structure
struct fuzzy_data
{
	double ipc;
	double powErr;

	int active_cores;
	int freq;

	bool request;
};

int main(int argc, char* argv[])
{
	double powerErr;
	key_t key = ftok("shmfile", 65); // ftok to generate unique key
	int shmid = shmget(key, 1024, 0666|IPC_CREAT); // shmget returns an identifier in shmid
	fuzzy_data *client = (fuzzy_data*) shmat(shmid, (void*)0, 0); // shmat to attach to shared memory
	sem_t *req_sem, *ack_sem;

	// Initialize semaphore with 0
	if((req_sem = sem_open(SEM_REQUEST_SIGNAL_NAME, 0, 0, 0)) == SEM_FAILED)
		printf("sem_open failed : req_sem");

	// counting semaphore, indicating the number of strings to be printed. Initial value=0
	if((ack_sem = sem_open(SEM_ACK_SIGNAL_NAME, 0, 0, 0)) == SEM_FAILED)
		printf("sem_open failed : ack_sem");

	//scanf powErr
	printf("Power Err -> ");
	scanf("%lf", &powerErr);
	client->powErr = powerErr;

	// Send request
	client->request = true;

	// get an ack
	while(sem_trywait (ack_sem) == -1)
		printf("Waiting for fuzzy");

	client->request = false;

	printf("\nresult\nfreq:%d\nEnd MARS\n", client->freq);

	return 0;
}
