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
  int client=0;
  
  while(1){
    if((client=accepte_client(socket_serveur))!=-1){
      int pid=fork();
      if(pid!=0){
	close(client);
      }
      else{
	traiter_client(client);
     
	close(client);
	exit(0);
      }
    }
  }
  return 0;
}
void traiter_client(int client){
  /*On peut maintenant dialoguer avec le client*/
 
  printf("Connexion\n");
  int erreur=-1;
  FILE *file = fdopen(client,"w+");
 
  char entete[1024];

  erreur=checkErreur(file,entete); 
     int fini=0;
  while(fini==0&&fgets(entete,sizeof(entete),file)!=NULL){
    if(strcmp(entete,"\r\n")==0||strcmp(entete,"\n")==0){
      fini=1;
    }
  }
  if(erreur>0){
  sendError(file,erreur);
  exit(6);
  }
  else{
    sendHello(file);
  }
  while(1){
      fini=0;
      if((erreur=checkErreur(file,entete))==0){
      while(fini==0&&fgets(entete,sizeof(entete),file)!=NULL){
	if(strcmp(entete,"\r\n")==0||strcmp(entete,"\n")==0){
	  fini=1;
	}
      }
    }
    else{
      sendError(file,erreur);
    }
    
  }

}
void sendError(FILE* file,int i){
  const char* erreur="";
  switch(i){
    
  case 400:
    erreur="HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400Bad Request\r\n";
    break;
  case 404:
    erreur="HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 20\r\n\r\n404 File Not Found\r\n";
    break;
  default :
    erreur="HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400Bad Request\r\n";
    break;
  }

  fprintf(file,erreur);
  fflush(file);
}

void sendHello(FILE* file){
  const char* hello="HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 23\r\n\r\nBienvenue sur Pinky !\r\n";
  fprintf(file,hello);
  fflush(file);
}

int checkErreur(FILE* file,char* entete){
  int i=0;
  if((i=strlen(fgets(entete,1024,file)))==-1){
    /*sendError(file,i);
      perror("fgets");
      exit(6);*/
    return i;
  }

  if((i=test_get(entete))!=0){
    //perror("i");
    /*sendError(file,i);
      exit(8);*/
    return i;
  }
  return 0;
}
