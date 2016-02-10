#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>

int main ()
{
  int socket_serveur=creer_serveur(8080);
  int client=-1;
  char buffer[1024];
  while(1){
    sleep(1);
    if((client=accepte_client(socket_serveur))!=-1){
      while(strcmp("FIN\n",buffer)!=0){	
	int i = 0;
	if((i=read(client,buffer,sizeof(buffer)))==-1){
	  perror("read");
	}

	buffer[i]='\0';
	
	if(write(client,buffer,i)==-1){
	  perror("write");
	}
      }
      close(client);      
    }
  }
  return 0;
}
