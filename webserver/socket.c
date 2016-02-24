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
  FILE *file = fdopen(socket_client,"w+");

  int i = 0;
  char entete[1024];
  
  if((i=strlen(fgets(entete,sizeof(entete),file)))==-1){
    perror("fgets");
    exit(6);
  }
  
  if(test_get(entete)<0){
    perror("test_get");
  }

  //fclose(file);
  
  /*On peut maintenant dialoguer avec le client*/
  /* const char* message="Salut poto comment tu vas ? Moi je payze dans le milieu je suis un mec bien et je suis avec mon poto Fitoussi, un bon gars assez sympa, même si il a une religion assez bizarre mais je crois pas que ce soit sa faute, on choisit pas tout dans la vie. Sinon moi j'suis juste un petit caca qui cherche à s'amuser en faisant le con sur le PC des autres, genre je m'amuse beaucoup très fort, et un jour et bah même que j'en ai fait rager plusieurs en une fois et que j'éatais trop content de moi (RIEN A FOUTRE DES FAUTES DE FRAPPE), et bah même que cette phrase et beaucoup trop longue et qu'il faudrait que je pense à mettre un point. Voilà. Maintenant du coup vu que t'es arrivé sur notre serveur et bah on va bien s'amuser vu qu'on a récupéré ton adresse IP et tout plein d'autres infos sur toi, et même que je vais pouvoir m'amuser à jouer avec ton PC. Sinon on t'as déjà parlé de la otoute puissance du Christ cosmique ? Non ? C'est normal on y connait rien, mais bon. J'AI BESOIN DE CAFE AAAAAAAAAAAH.";
     write(socket_client,message,strlen(message));
  */
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
