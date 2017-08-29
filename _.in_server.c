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
#include "trie.h"
#include "connection.h"
#define PORT 9091


struct dnsnode* root_in;

void * threadFunc(void * socket)
{  
	char buffer[1024] = {0}, send_buffer[1024] = {0};
    char * present = (char*) malloc(1024*sizeof(char));
    int new_socket = *(int*)socket;
 	read(new_socket, buffer,1024);
    int iterate = atoi(buffer);
 
    for(int i = 0 ; i < iterate ; i++)
     {
            //printf("in the loop");
            //printf("before reading buffer");
            int x = read(new_socket, buffer,1024);
            //printf("%s\n",buffer);
            present = search(root_in,buffer);
            if(!present)
            {
                    present = "N";
                    //printf("%s after search results in .in server\n",present);
                    send(new_socket,present,1024, 0);
            }  

    }  

}

int main(int argc, char const *argv[])
{
    
    root_in  = newnode();
    insert_in_tree(root_in,"insert_in.txt");
    pthread_t pth;
    int rc; 
    int server_fd, new_socket,*new_sock, valread;
    struct sockaddr_in address;
    int opt = 1;
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
    address.sin_port = htons(PORT);
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
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
           
        	new_sock = malloc(sizeof *new_sock);
            *new_sock = new_socket;
            rc=pthread_create(&pth,NULL,threadFunc,(void *)new_sock);  
        }

    }

    return 0;

}

