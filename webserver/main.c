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
  FILE* file=fdopen(client,"w+");
   /*On peut maintenant dialoguer avec le client*/
   /*const char* message="Salut poto comment tu vas ? Moi je payze dans le milieu je suis un mec bien et je suis avec mon poto Fitoussi, un bon gars assez sympa, même si il a une religion assez bizarre mais je crois pas que ce soit sa faute, on choisit pas tout dans la vie. Sinon moi j'suis juste un petit caca qui cherche à s'amuser en faisant le con sur le PC des autres, genre je m'amuse beaucoup très fort, et un jour et bah même que j'en ai fait rager plusieurs en une fois et que j'éatais trop content de moi (RIEN A FOUTRE DES FAUTES DE FRAPPE), et bah même que cette phrase et beaucoup trop longue et qu'il faudrait que je pense à mettre un point. Voilà. Maintenant du coup vu que t'es arrivé sur notre serveur et bah on va bien s'amuser vu qu'on a récupéré ton adresse IP et tout plein d'autres infos sur toi, et même que je vais pouvoir m'amuser à jouer avec ton PC. Sinon on t'as déjà parlé de la otoute puissance du Christ cosmique ? Non ? C'est normal on y connait rien, mais bon. J'AI BESOIN DE CAFE AAAAAAAAAAAH.";
    */
while(strcmp("FIN\n",buffer)!=0){	
	  int i = 0;
	  // if((i=read(client,buffer,sizeof(buffer)))==-1){
	  if((i=strlen(fgets(buffer,sizeof(buffer),file)))==-1){
	    perror("fgets");
	    break;
	  }
	
	  if(i==0)break;

	  /*
	  if(fprintf(file,"%s %s","<PINKY>",buffer)==-1){
	    perror("fprintf");
	    break;
	  }
	  */
	  if(printf("%s %s","<PINKY>",buffer)==-1){
	    perror("fprintf");
	    break;
	  }
	}
}
