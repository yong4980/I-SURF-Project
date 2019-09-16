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

#define PORT 9999
#define HOSTIP "169.234.54.68"
#define MAXCLIENT 30
#define MAXSOCKET MAXCLIENT+1

class Tcp{
private:

  struct sockaddr_in serverAddr; //Server Info
  struct sockaddr_in newClientAddr; //New Client Info
  socklen_t addr_size;
  char* buffer;
  int clientSocket, check;
  int* connectedSocket;

  key_t key;
  int shmid;

public:
  Tcp();
  ~Tcp();
  int BuildServerTCP();
  int BuildClientTCP();
  void ListeningClient(int newSocketNum);
  char* ReadMsg(int socketNum);
  char* ReadMsg(int socket, int client);
  void WriteMsg(int socketNum, char* msg);
  void WriteMsg(int socket, char* msg, int client);
  void WriteMsg(char* socketNumStr, char* msg); //Function overload
  int FindEmptySocket();
  void CheckConnectedSocket();
  int GetMaxClientNum();
  void QuitTCP();
  void QuitAll();
  void close(int socketNum);
  void DisconnectSocket(int socketNum);

  void setBuffer(char* buffer);
  char* getBuffer();
};

#endif
