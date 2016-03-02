#include <stdio.h>
#include "socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int creer_serveur(int port){
  int socket_serveur;
  socket_serveur=socket(AF_INET,SOCK_STREAM,0);
  if(socket_serveur==-1){
    perror("socket_serveur");
    exit(1);
  }
  int optval = 1;
  if ( setsockopt ( socket_serveur , SOL_SOCKET , SO_REUSEADDR , & optval , sizeof ( int )) == -1){
    perror ( " Can not set SO_REUSEADDR option " );
  }

  initialiser_signaux();
  
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET; /*Socket ipv4*/
  saddr.sin_port = htons(port); /*Port */
  saddr.sin_addr.s_addr= INADDR_ANY; /*écoute sur toutes les interfaces */

  if(bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
    perror("bind socket_serveur");
    exit(2);
  }
  if(listen(socket_serveur,10)==-1){
    perror("listen socket_serveur");
    exit(3);
  }

  
  
  
  return socket_serveur;
}

void initialiser_signaux ( void ){

  if ( signal ( SIGPIPE , SIG_IGN ) == SIG_ERR )
    {
      perror ( " signal " );
    }
  struct sigaction sa ;
  sa . sa_handler = traitement_signal ;
  sigemptyset (& sa . sa_mask );
  sa . sa_flags = SA_RESTART ;
  if ( sigaction ( SIGCHLD , & sa , NULL ) == -1)
    {
      perror ( " sigaction ( SIGCHLD ) " );
    }

}

int accepte_client(int socket_serveur){
  int socket_client;
  socket_client=accept(socket_serveur,NULL,NULL);
  if(socket_client==-1){
    perror("accept");
    exit(4);
  }

 
  
  return socket_client;
}

void traitement_signal ( int sig )
{
  //  printf ("Signal %d recu \n" , sig );
  wait(&sig);
}

int test_get(char buf[])
{
  int i=0;
  int cpt=0;
  int idx;
  if(buf[0]!='G' || buf[1]!='E' || buf[2]!='T'){
    return -1;
  }
  while(buf[i]!='\0'){
    if(buf[i]==' '){
      cpt++;
      idx=i;
    }
    i++;
  }

  if(cpt==2){
      cpt=0;
       int j;
       int taille=(i-2)-(idx+1);
       if(taille!=8){
	 return -3;
       }
       
       char mot_trois[taille+1];
       char * http1="HTTP/1.1";
       char * http2="HTTP/1.0";

       for(j=idx+1;j<idx+taille+1;j++){
	 mot_trois[cpt]=buf[j];
	 cpt++;
       }
       mot_trois[taille]='\0';
       if(strcmp(mot_trois,http1)==0 || strcmp(mot_trois,http2)==0){
	 return 0;
       }
       return -4;    
  }
  return -2;
  
}
