#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <stdlib.h>
enum  http_method {
  HTTP_GET ,
  HTTP_UNSUPPORTED ,
};

typedef  struct
{
  enum  http_method  method;
  int   major_version;
  int   minor_version;
  char *url;
} http_request;
void traiter_client(int client);
/*
void sendError(FILE* file,int i);
void sendHello(FILE* file);
int checkErreur(FILE* file, char* entete);
*/
char *fgets_or_exit(char *buffer,int size,FILE *stream);
int parse_http_request(const char* request_line, http_request *request);
void skip_headers(FILE *file);
void send_status(FILE *client,int code,const char *reason_phrase);
void send_response(FILE *client,int code,const char *reason_phrase,const char* message_body);
int check_repert_valide(const char* chemin);
char* rewrite_url(char* url);
int check_and_open(const char* url,const char* document_root);
int get_file_size(int fd);
char * gettype(char  nom[]);
int copy(int in, int out);
void send_response_fd(FILE *client,int code,const char *reason_phrase,int fd,int out);
