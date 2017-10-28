// Client side C/C++ program to demonstrate Socket programming
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
#include <iostream>
#include <errno.h>
#include <arpa/inet.h> 
#include <pthread.h>
#define PORT 4000

using namespace std;
int flag = 1;

void * threadfunc( void * threadid)
{

    //std::cout << "\n\n\n hello......." << count << "\n";
   int thread = *(int*)threadid;
   int x =0;
 
   while(flag)
   {

    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        //return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        //return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        //return -1;
    }

 
        clock_t first = clock();
        //send(sock , "hello" , 10 , 0 );
        //while(read( sock , buffer, 1024)<0);
        float secsElapsed = clock();
        printf("\n %f %s %d \n",(secsElapsed-first),buffer, thread);
        //sleep(1);
        //shutdown(sock);
        close(sock);
        //cout<<"Socket closed "<<*thread<<endl;
    }


    //cout << "no of request" << x << endl;
}

int main(int argc, char const *argv[])
{


    int i,c;
    int num1;
    int * i1 = (int *)malloc(sizeof *i1);
    int num = atoi(argv[1]);
    pthread_t pth[num];
    //long long int experiment = atoi(argv[3]);
    int errCode[100] = {0}; 


    for(int j =0 ;j < num ;j++)
    {
     
        //clock_t startTime1 = clock();
            *i1 = j;
            if((errCode[j] = pthread_create(&pth[j], NULL, threadfunc, (void*)i1)) != 0)
                printf( "thread is not created\n");

    }

    sleep(atoi(argv[2]));

    flag = 0;
    cout<<"\n Time out after sleep\n";
        
    for( int i1 = 0; i1 < num; i1++ ) 
    {
        
        if(errCode[i1] == 0)
        {
            errCode[i1] = pthread_join(pth[i1], NULL);
        
            if (errCode[i1]) 
            {
                cout << "Error:unable to join," << errCode[i1] << endl;
                exit(-1);
            }
        }
    }

   pthread_exit(NULL);


    
    
    return 0;

}
