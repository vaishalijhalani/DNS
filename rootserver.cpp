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
#include <unordered_map>
#include "connection.h"
#include "hashmap.h"

using namespace std;
#define PORT 8081


unordered_map <std::string, std::string> hash_root;

void * threadFunc(void * socket)
{  

	
    int dest_port;
    char *buffer = (char*) malloc(1024*sizeof(char));
    char *send_buffer1 = (char*) malloc(1024*sizeof(char));
    std::string present(1024,0);
    int new_socket = *(int*)socket;

	int x = read(new_socket, buffer,1024);
	//printf("%s is the read data by root\n",buffer);
	std::string send_buffer(buffer);
	present = search(hash_root,send_buffer);
           
    //cout << present << "\n after search results\n";
    if (!present.empty())
       dest_port = set_port(buffer); 
	
    else
       dest_port = 0; 	 


    snprintf (send_buffer1, sizeof(send_buffer1), "%d",dest_port);
    puts(send_buffer1);
    send(new_socket,send_buffer1,1024, 0);
    
    free(buffer);
    free(send_buffer1);
    free(socket);
    close(new_socket);
    pthread_exit(NULL);
    


}


int main(int argc, char const *argv[])
{

    
    //int i = 0, j = 0;
    const char * file = (char*) malloc(1024*sizeof(char));
    file = "root.txt";
    pthread_t pth;
    int rc; 
    int opt=1;
    // Creating socket file descriptor
    int server_fd, new_socket, valread,*new_sock;
    struct sockaddr_in address;
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
    if (listen(server_fd, SOMAXCONN) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    insert_in_hash(hash_root, file);
   // cout << "values in hashmap\n";
    //for(unordered_map<std::string,std::string>::iterator it = hash_root.begin(); it != hash_root.end(); ++it) {
 
      //  cout <<"value in hash\n" << it->first << " " << it->second;}

	while(1)
	{

	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
	                       (socklen_t*)&addrlen))<0)
	    {
	        perror("accept");
	        exit(EXIT_FAILURE);
	    }

	    if (new_socket>0)
	    {
            //printf("\nroot server data.........\n");
             //pthread_attr_t attr; // thread attribute
// set thread detachstate attribute to DETACHED 
            //pthread_attr_init(&attr);
            //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	        new_sock = (int *)malloc(sizeof *new_sock);
            *new_sock = new_socket;
            rc=pthread_create(&pth,NULL,threadFunc,(void *)new_sock);	
            if(rc == 0 )
            pthread_detach(pth);   
        }
	    
	}

     return 0;
}