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
  saddr.sin_addr.s_addr= INADDR_ANY; /*�coute sur toutes les interfaces */

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
  /*On peut maintenant dialoguer avec le client*/
  /* const char* message="Salut poto comment tu vas ? Moi je payze dans le milieu je suis un mec bien et je suis avec mon poto Fitoussi, un bon gars assez sympa, m�me si il a une religion assez bizarre mais je crois pas que ce soit sa faute, on choisit pas tout dans la vie. Sinon moi j'suis juste un petit caca qui cherche � s'amuser en faisant le con sur le PC des autres, genre je m'amuse beaucoup tr�s fort, et un jour et bah m�me que j'en ai fait rager plusieurs en une fois et que j'�atais trop content de moi (RIEN A FOUTRE DES FAUTES DE FRAPPE), et bah m�me que cette phrase et beaucoup trop longue et qu'il faudrait que je pense � mettre un point. Voil�. Maintenant du coup vu que t'es arriv� sur notre serveur et bah on va bien s'amuser vu qu'on a r�cup�r� ton adresse IP et tout plein d'autres infos sur toi, et m�me que je vais pouvoir m'amuser � jouer avec ton PC. Sinon on t'as d�j� parl� de la otoute puissance du Christ cosmique ? Non ? C'est normal on y connait rien, mais bon. J'AI BESOIN DE CAFE AAAAAAAAAAAH.";
     write(socket_client,message,strlen(message));*/
  return socket_client;
}
void traitement_signal ( int sig )
{
  //  printf ("Signal %d recu \n" , sig );
  wait(&sig);
}

