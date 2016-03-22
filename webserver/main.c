#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <stdlib.h>
#include "main.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char* server_name = "<PINKY>";
char* hello_msg="Bienvenue sur Pinky !\r\n";
http_request request;
char* chemin="";
int main (int argc,char** argv)
{
  int socket_serveur=creer_serveur(8080);
  int client=0;
  if(argc<2){
    printf("Impossible de lancer sans fichier root\n");
  }
  else if(check_repert_valide(rewrite_url(argv[1]))==-1){
    printf("Erreur dans l'url\n");
  }
  else{
    chemin=rewrite_url(argv[1]);
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
  }
    return 0;
}
void traiter_client(int client){
  /*On peut maintenant dialoguer avec le client*/
 
  printf("Connexion\n");
  int erreur=-1;
  FILE *file = fdopen(client,"w+");
  int fd;

  char entete[1024];

  erreur=parse_http_request(fgets_or_exit(entete,sizeof(entete),file),&request);
  printf("%s\n",request.url);
  skip_headers(file);
  
  if(strcmp(request.url,"ERREUR")==0){
    send_response(file,403,"Forbidden", "Forbidden\r\n");
    
  }
  else if(erreur==0){
    send_response(file,400,"Bad Request", "Bad Request\r\n");
  }
  else if(request.method==HTTP_UNSUPPORTED){
    send_response(file,405,"Method Not Allowed","Method Not Allowed\r\n");
  }
  else if((fd=check_and_open(request.url,chemin))!=-1){
    send_response_fd(file,200,"OK",fd,client);
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

  while(token) {
    ++cpt;
    if(cpt==1 && strcmp(token,"GET")==0)
      {	  
	request->method = HTTP_GET;
      }
    if(cpt==2)
      {
	request->url = rewrite_url(token);
	printf("Url : %s\n",request->url);
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
    printf("ERREUR tour %d : %d, token=%s\n",cpt,ret,token);
    token=strtok(NULL," ");

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
  fprintf(client,"Connection: close\r\nContent-Length: %zu\r\nContent-type: %s\r\n\r\n%s",strlen(message_body),gettype(request.url),message_body);
  fflush(client);
}

void send_response_fd(FILE *client,int code,const char *reason_phrase,int fd,int out){
  send_status(client,code,reason_phrase);
  fprintf(client,"Connection: close\r\nContent-Length: %d\r\nContent-type: %s\r\n\r\n",get_file_size(fd),gettype(request.url));
  fflush(client);
  copy(fd,out);
}

int check_repert_valide(const char* chemin){
  struct stat file_stat;
  printf("%s\n",chemin);
  if(stat(chemin,&file_stat)==-1){
    perror("stat");
    return -1;
  }
  if(S_ISDIR(file_stat.st_mode)==0){
    printf("Ce n'est pas un répertoire !\n");
    return -1;
  }
  if(((S_IXUSR & file_stat.st_mode)==0)||((S_IXOTH & file_stat.st_mode)==0)){
    printf("Impossible d'accéder au répertoire !\n");
    return -1;
  }
  return 0;
}
char *rewrite_url(char* url){
  if(strcmp(url,"/")==0){
    return "/index.html";
  }
  

  char* token="";
  token=strtok(url,"/");
  while(token){
    if(strcmp(token,"..")==0){
      return "ERREUR";
    }

    token=strtok(NULL," ");
  }
  
  return strtok(url,"?");
}
int check_and_open(const char* url,const char* document_root){
  char * chemin=malloc(strlen(url+strlen(document_root)+1));
  strcpy(chemin,document_root);
  strcat(chemin,url);
  FILE *fichier=NULL;
  fichier=fopen(chemin,"r");
  if(fichier==NULL){
    perror("le fichier n'existe pas");
    return -1;
  }
  struct stat statdata;
  stat(chemin,&statdata);
  if(!S_ISREG(statdata.st_mode)){
    perror("ce n'est pas un fichier régulier");
    return -1;
  }
  int fd=open(chemin,O_RDONLY);
  return fd;
}

int get_file_size(int fd){
  struct stat statbuf;
  fstat(fd,&statbuf);
  return statbuf.st_size;
}
char * gettype(char  nom[]){
  int k=strlen(nom);
  int i=0;
  
  while(i<k && nom[i]!='.'){
    i++;
  }
  if(nom[i]=='.'){
    i++;
  }
  int j=0;
  char ext[strlen(nom)-i];
  while(i+j<=k  ){
    ext[j]=nom[i+j];
    j++;
  }
  printf("ext : %s\n",ext);
  fflush(stdout);
  if(strcmp(ext,"jpg")==0){
    return "image/jpeg";
  }
  if(strcmp(ext,"jpeg")==0){
    return "image/jpeg";
  }
  if(strcmp(ext,"html")==0){
    return "text/html";
  }
  if(strcmp(ext,"htm")==0){
    return "text/html";
  }
  return "text/plain";
}
int copy(int in, int out){

  char * buff = malloc(get_file_size(in));
  if(read(in,buff,get_file_size(in))==-1){
    perror("read");
    return 0;
  }
  if(write(out,buff,get_file_size(in))==-1){
    perror("write");
    return 0;
  }
  return 1;
}
