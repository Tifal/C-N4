#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <stdlib.h>
#include "main.h"
int main ()
{
  int socket_serveur=creer_serveur(8080);
  int client=-1;
  
  while(1){
    sleep(1);
    if((client=accepte_client(socket_serveur))!=-1){
      int pid=fork();
      if(pid!=0){
	close(client);
      }
      else{
	/*while(strcmp("FIN\n",buffer)!=0){	
	  int i = 0;
	  if((i=read(client,buffer,sizeof(buffer)))==-1){
	    perror("read");
	    break;
	  }
	
	  if(i==0)break;
	
	  buffer[i]='\0';
	
	  if(write(client,buffer,i)==-1){
	    perror("write");
	    break;
	  }
	  }*/
	traiter_client(client);
     
	close(client);
	exit(0);
      }
    }
  }
  return 0;
}
void traiter_client(int client){
  char buffer[1024];
while(strcmp("FIN\n",buffer)!=0){	
	  int i = 0;
	  if((i=read(client,buffer,sizeof(buffer)))==-1){
	    perror("read");
	    break;
	  }
	
	  if(i==0)break;
	
	  buffer[i]='\0';
	
	  if(write(client,buffer,i)==-1){
	    perror("write");
	    break;
	  }
	}
}
