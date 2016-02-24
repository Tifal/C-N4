#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <stdlib.h>
#include "main.h"

char* server_name = "<PINKY>";

int main ()
{
  int socket_serveur=creer_serveur(8080);
  FILE* client=NULL;
  
  while(1){
    if((client=accepte_client(socket_serveur))!=NULL){
      int pid=fork();
      if(pid!=0){
	fclose(client);
      }
      else{
	traiter_client(client);
     
	fclose(client);
	exit(0);
      }
    }
  }
  return 0;
}
void traiter_client(FILE* file){
  char buffer[1024];  
  /*On peut maintenant dialoguer avec le client*/
  const char* message="Salut poto comment tu vas ? Moi je payze dans le milieu je suis un mec bien et je suis avec mon poto Fitoussi, un bon gars assez sympa, même si il a une religion assez bizarre mais je crois pas que ce soit sa faute, on choisit pas tout dans la vie. Sinon moi j'suis juste un petit caca qui cherche à s'amuser en faisant le con sur le PC des autres, genre je m'amuse beaucoup très fort, et un jour et bah même que j'en ai fait rager plusieurs en une fois et que j'éatais trop content de moi (RIEN A FOUTRE DES FAUTES DE FRAPPE), et bah même que cette phrase et beaucoup trop longue et qu'il faudrait que je pense à mettre un point. Voilà. Maintenant du coup vu que t'es arrivé sur notre serveur et bah on va bien s'amuser vu qu'on a récupéré ton adresse IP et tout plein d'autres infos sur toi, et même que je vais pouvoir m'amuser à jouer avec ton PC. Sinon on t'as déjà parlé de la otoute puissance du Christ cosmique ? Non ? C'est normal on y connait rien, mais bon. J'AI BESOIN DE CAFE AAAAAAAAAAAH.";
  printf("%s\n",message);
   
  while(strcmp("FIN\n",buffer)!=0){
    int i = 0;
    // if((i=read(client,buffer,sizeof(buffer)))==-1){
    if((i=strlen(fgets(buffer,sizeof(buffer),file)))==-1){
      perror("fgets");
      break;
    }
	
    if(i==0)break;
    
    if(fprintf(file,"%s %s",server_name,buffer)==-1){
      perror("fprintf");
      break;
    }
    /*
      if(printf("%s %s","<PINKY>",buffer)==-1){
      perror("printf");
      break;
      }
    */
  }
}
