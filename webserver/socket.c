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
  wait(&sig);
}
