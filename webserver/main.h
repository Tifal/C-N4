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
void sendError(FILE* file,int i);
void sendHello(FILE* file);
int checkErreur(FILE* file, char* entete);
char *fgets_or_exit(char *buffer,int size,FILE *stream);
int parse_http_request(const char* request_line, http_request *request);
void skip_headers(FILE *file);

