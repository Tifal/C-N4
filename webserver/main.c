#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>

int main ()
{
  int socket_serveur=creer_serveur(8080);
  int client=-1;
  int ok=1;
  char buffer[1024];
  while(1){
  sleep(1);
  if((client=accepte_client(socket_serveur))!=-1){
    while(ok==1){
      int i = 0;
      if((i=read(client,buffer,sizeof(buffer)))==-1){perror("read");}
	 if(write(client,buffer,i)==-1){
	perror("write");
      }
    }
  }
  }
return 0;
}
