#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <stdlib.h>
#include "main.h"

char* server_name = "<PINKY>";
http_request request;
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

  erreur=parse_http_request(fgets_or_exit(entete,sizeof(entete),file),&request);
  skip_headers(file);
  
  if(erreur==0){
  sendError(file,400);
  exit(400);
  }
  else if(strcmp(request.url,"/")==0){
    sendHello(file);
    exit(0);
  }
  else{
    sendError(file,404);
    exit(404);
  }
  // while(1){
  /*     fini=0;
      if((erreur=checkErreur(file,entete))==0){
      while(fini==0&&fgets_or_exit(entete,sizeof(entete),file)!=NULL){
	if(strcmp(entete,"\r\n")==0||strcmp(entete,"\n")==0){
	  fini=1;
	}
      }
    }
    else{
      sendError(file,erreur);
    }
  */
      //    }

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

char *fgets_or_exit(char *buffer,int size,FILE *stream)
{
  char* message=fgets(buffer, size, stream);
  if(message==NULL){
    printf("La connexion s'est interrompue !");
    exit(2);
  }
  return message;
}

int parse_http_request(const char* request_line, http_request *request){
  char* token ="";
  char* reqdup = strdup(request_line);
  int ret = 1;
  int cpt = 0;
  token = strtok(reqdup, " ");
  request->method = HTTP_UNSUPPORTED;

  while(token)
    {
      ++cpt;
      if(cpt==1 && strcmp(token,"GET")==0)
	{
	  
	  request->method = HTTP_GET;
	}
      if(cpt==2)
	{
	  request->url = token;
	}	      
      if(cpt > 3)
	{
	  ret = 0;
	}
      if(cpt == 3)
	{
	  if(strcmp(token,"HTTP/1.0\r\n")==0)
	    {
	      request->major_version = 1;
	      request->minor_version = 0;
	    }
	  else if (strcmp(token,"HTTP/1.1\r\n")==0)
	    {
	      request->major_version = 1;
	      request->minor_version = 1;
	    }
	  else
	    {
	      ret = 0;
	    }
	}
      token=strtok(NULL," ");
    } 
  if(cpt < 3 )
    {
      ret = 0;
    }
  return ret;
}
void skip_headers(FILE *file){
  char entete[1024];
  int fini=0;
  while(fini==0&&fgets_or_exit(entete,sizeof(entete),file)!=NULL){
    if(strcmp(entete,"\r\n")==0||strcmp(entete,"\n")==0){
      fini=1;
    }
  }
}
