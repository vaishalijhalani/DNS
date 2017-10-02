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
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8080

void * threadFunc(void * socket)
{  
        int new_socket = *(int*)socket;
        int x;
        char * buffer = (char*) malloc(1024*sizeof(char));
        while(read( new_socket , buffer, 1024)<0);
        send(new_socket , "hello", 1024 , 0 );
        printf("Hello message sent\n");
        free(buffer);
        free(socket);
        close(new_socket);
        pthread_exit(NULL);
       
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
            
            new_sock = (int*)malloc(sizeof *new_sock);
            *new_sock = new_socket;
            rc=pthread_create(&pth,NULL,threadFunc,(void *)new_sock); 
            if(rc == 0)
            pthread_detach(pth); 
        }

    }

   
    return 0;
}
