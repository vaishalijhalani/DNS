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

int count = 0;

char url[11][100] ={
"www.samsung.com",
"www.samsung.com",
"www.samsung.com",
"www.samsung.com",
"www.samsung.com",
"www.samsung.com",
"www.samsung.com",
"www.samsung.com",
"www.samsung.com",
"www.samsung.com",
"www.samsung.com"};

void * threadfunc( void * threadid)
{

    //std::cout << "\n\n\n hello......." << count << "\n";
    int thread = *(int*)(threadid);
    char  * url_search  = (char *)malloc (1024 * sizeof(char));
    char read_buffer[1024] = {0};
    char line[1024] = {0};
    int sock = client_initialise(PORT);
    clock_t startTime = clock();
    send(sock,url[count],1024, 0);
    while(read(sock,read_buffer,1024)<0);
    float secsElapsed = (float)(clock() - startTime)/CLOCKS_PER_SEC;
    std::cout << secsElapsed*1000 << " " << thread << endl;
/*
    if(!strcmp(read_buffer,"N"))
        
        std::cout << thread << ": " << url[count] << " is not a valid url\n";

    else if(!strcmp(read_buffer,"D"))
        
        std::cout << thread << ": " <<  url[count] << " is not a valid domain\n";

    else
      
        std::cout << thread << ": " <<  read_buffer <<  " is the ip address for " << url[count] << endl;
*/
    count++;
    if(count > 10) count = 0;

    close(sock);
    shutdown(sock,SHUT_RDWR);
    

}
  
int main(int argc, char const *argv[])
{
    int i;
    int * i1 = (int *)malloc(sizeof *i1);
    pthread_t pth;
    int experiment = 0;
    int num = atoi(argv[1]);
    for(long long int j =0 ;j < 50 ;j++)
    {
    cout << experiment << endl;
    for(i=0 ; i<num; i++)
    {
        *i1 = i;

        if(pthread_create(&pth,NULL,threadfunc,(void *)i1)<0)
            std::cout << "thread is not created\n";
        
    
      //sleep(1); 
    } 
    sleep(1);
    experiment++;
    count = 0;
}


   pthread_exit(NULL);

    return 0;
}
