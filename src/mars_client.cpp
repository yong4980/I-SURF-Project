#include "mars_tcp.h"

int main(void){
  Tcp clientTcp;
  int tmp = clientTcp.BuildClientTCP();

  char* buffer;
  buffer = new char[1024];
  fgets(buffer, sizeof(buffer), stdin);

  buffer = clientTcp.ReadMsg(tmp);
  printf(" %s\n", buffer);
  clientTcp.WriteMsg(tmp, buffer);


delete[] buffer;
  return 0;
}
