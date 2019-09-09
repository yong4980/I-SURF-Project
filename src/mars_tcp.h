#ifndef _MARS_TCP_H_
#define _MARS_TCP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>

#define PORT 4444
#define HOSTIP "169.234.17.238"
#define MAXCLIENT 30

class Tcp{
private:

  struct sockaddr_in serverAddr; //Server Info
  struct sockaddr_in newClientAddr; //New Client Info
  socklen_t addr_size;
  char* checkStr;
  char* buffer;
  int listeningSocket, clientSocket, check, nbytes;
  int* connectedSocket;

  key_t key;
  int shmid;


public:
  Tcp();
  ~Tcp();
  int BuildServerTCP(); //done
  int BuildClientTCP(); //done
  int ListeningClient(); //done
  char* ReadMsg(int socket); //done
  void WriteMsg(int socket, char* msg); //done
  int FindEmptySocket(); //done

  void setBuffer(char* buffer);
  char* getBuffer();
};

#endif
