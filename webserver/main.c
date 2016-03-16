#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <stdlib.h>
#include "main.h"

char* server_name = "<PINKY>";
char* hello_msg="Bienvenue sur Pinky !\r\n";
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
    send_response(file,400,"Bad Request", "Bad Request\r\n");
  }
  else if(request.method==HTTP_UNSUPPORTED){
    send_response(file,405,"Method Not Allowed","Method Not Allowed\r\n");
  }
  else if(strcmp(request.url,"/")==0){
    send_response(file,200,"OK",hello_msg);
  }
  else{
    send_response(file,404,"Not Found","Not Found\r\n");
  }

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

void send_status(FILE *client,int code,const char *reason_phrase){
  fprintf(client,"HTTP/1.1 %d %s\r\n",code,reason_phrase);
  fflush(client);
}

void send_response(FILE *client,int code,const char *reason_phrase,const char* message_body){
  send_status(client,code,reason_phrase);
  fprintf(client,"Connection: close\r\nContent-Length: %zu\r\n\r\n%s",strlen(message_body),message_body);
  fflush(client);
}
