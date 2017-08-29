#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 



#ifndef _connection_h
#define _connection_h

int set_port(char * buffer);
int client_initialise(int port);
int turn_clientmode_on_for_root(char *buffer, int port);
char * turn_clientmode_on(char *buffer, int port);

#endif
