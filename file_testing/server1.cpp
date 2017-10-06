// Server side C/C++ program to demonstrate Socket programming
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
#include <iostream>
#include <queue>
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8080

using namespace std;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t handle = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t handle1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t count_dec = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

queue <int> p1;

int count = 0;

void * worker_thread(void * socket)
{


        char * buffer = (char*) malloc(1024*sizeof(char));
        int new_socket = *(int*)socket;
        while(read( new_socket , buffer, 1024)<0);
        send(new_socket , "hello", 1024 , 0 );
        cout << buffer << endl;
        printf("Hello message sent\n");
        

      //  pthread_mutex_lock(&count_dec);
        count--;
        //pthread_mutex_unlock(&count_dec);
       

        free(buffer);
        close(new_socket);
        pthread_detach(pthread_self());



}


void * Handler(void * temp)
{  
    
    int rc = 0;
    int * new_sock;
    pthread_t worker;
    cout <<"hello hendler\n";

    while(1)
    {
        
        if((count<=30) && (!p1.empty()))
        {
            cout <<"hello hendler\n";
           // pthread_mutex_lock(&handle);
            int socket = p1.front();
            p1.pop();
            //pthread_mutex_unlock(&handle);
            *new_sock = socket;
            
            rc = pthread_create(&worker,NULL,worker_thread,(void*) new_sock);
            //if(rc == 0)
              //  pthread_detach(worker);

        //    pthread_mutex_lock(&handle1);
            count++;
          //  pthread_mutex_unlock(&handle1);
            
        }

       

       
    }

    
    
       
}



int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int opt = 1;
    pthread_t pth;
    int rc; 
    int server_fd, new_socket,*new_sock, valread;
    int addrlen = sizeof(address);

      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, SOMAXCONN) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    
    pthread_create(&pth,NULL,Handler,NULL);

    while(1)
    {

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }


        if(new_socket>0)
        {
            //cout << new_socket << " pushing\n";
           // pthread_mutex_lock(&mtx);
            p1.push(new_socket);
            //pthread_mutex_unlock(&mtx);
           
        }

    }

   
    return 0;
}
