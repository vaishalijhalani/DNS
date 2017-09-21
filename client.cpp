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
#include <pthread.h>
#include <iostream>
//#include <unordered_map>
#include "connection.h"
#include "hashmap.h"

using namespace std;


#define PORT 8080
#define NUM 5

FILE * file2;

void * threadfunc( void * threadid)
{

    std::cout << "\n\n\n hello.......\n";
    int thread = *(int*)(threadid);
    char  * url_search  = (char *)malloc (1024 * sizeof(char));
    char read_buffer[1024] = {0};
    char line[1024] = {0};
    int sock = client_initialise(PORT);
    int i1,x;
    url_search = (char *)malloc(50*sizeof(char));
    fgets ( line, sizeof(line), file2);
            
    for( i1 =0;i1<strlen(line)-1;i1++) 

            url_search[i1] = line[i1];

    url_search[i1] = '\0';
    cout << url_search;
    send(sock,url_search ,1024, 0);
    std::cout << "\n msg sent by client\n";
    while(read(sock, read_buffer,1024)<0);

    //cout << read_buffer << " after read \n";

    if(!strcmp(read_buffer,"N"))
        
        std::cout << url_search << " is not a valid url\n";

    else if(!strcmp(read_buffer,"D"))
        
        std::cout << url_search << " is not a valid domain\n";

    else
      
        std::cout << read_buffer <<  " is the ip address for " << url_search << endl;

    close(sock);

}
  
int main(int argc, char const *argv[])
{
    int i;
    int * i1 = (int *)malloc(sizeof *i1);
    file2 = fopen ( "search.txt", "r" );
    pthread_t pth;
    for(i=0 ; i<=NUM ; i++)
    {
        *i1 = i;
        if(pthread_create(&pth,NULL,threadfunc,(void *)i1)<0)
            std::cout << "thread is not created\n";
        
        sleep(2);
    }


   pthread_exit(NULL);

    return 0;
}
