#include <stdio.h>
#include <sys/socket.h>
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
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h> 
#include "connection.h"
#include "trie.h"

#define PORT 8080
#define NUM 5

FILE * file2;

void * threadfunc( void * threadid)
{

    printf("\n\n\n hello.......\n");
    int thread = *(int*)(threadid);
    char  * url_search  = malloc (1024 * sizeof(char));
    char read_buffer[1024] = {0};
    char line[1024] = {0};
    int sock = client_initialise(PORT);
    //printf("Enter url address for thread %d\n",thread);
    int i1;
    url_search = (char *)malloc(50*sizeof(char));
    fgets ( line, sizeof(line), file2);
            
    for( i1 =0;i1<strlen(line)-1;i1++) 

            url_search[i1] = line[i1];

    url_search[i1] = '\0';

    //strcpy(url_search,p) ;

    send(sock,url_search ,1024, 0 );
    printf("\n msg sent by client\n");
    read(sock, read_buffer,1024);
    //printf("\n %d after read value of x\n",x);

    if(!strcmp(read_buffer,"N"))
        
        printf("\n %s is not a valid url\n", url_search);

    else if(!strcmp(read_buffer,"D"))
        
        printf("\n %s is not a valid domain\n", url_search);

    else
        
        printf("\n %s is the ip address for %s\n", read_buffer,url_search);

}
  
int main(int argc, char const *argv[])
{
    int i;
    int * i1 = malloc(sizeof *i1);
    file2 = fopen ( "search.txt", "r" );
    pthread_t pth;
    for(i=0 ; i<=NUM ; i++)
    {
        *i1 = i;
        if(pthread_create(&pth,NULL,threadfunc,(void *)i1)<0)
            printf("thread is not created\n");
        
        sleep(2);
    }


   pthread_exit(NULL);

    return 0;
}
