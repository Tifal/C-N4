#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <stdlib.h>
void traiter_client(int client);
void sendError(FILE* file,int i);
void sendHello(FILE* file);
