#include "mars_tcp.h"

int main(void){
  Tcp serverTcp;
  serverTcp.BuildServerTCP();
  int tmp = serverTcp.ListeningClient();

  char* buffer;
  buffer = new char[1024];
  fgets(buffer, sizeof(buffer), stdin);

  serverTcp.WriteMsg(tmp, buffer);
  buffer = serverTcp.ReadMsg(tmp);
  printf(" %s\n", buffer);

  delete[] buffer;
  return 0;
}
