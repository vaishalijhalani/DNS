#include <bits/stdc++.h>
#include <tr1/unordered_map>
#include "connection.h"
#include "hashmap.h"

using namespace std::tr1;
#define PORT 8081


unordered_map <char *, char *> hash_root;

void * threadFunc(void * socket)
{  

	char buffer[1024] = {0}, send_buffer[1024] = {0};
    int dest_port;
    char * present = (char*) malloc(1024*sizeof(char));
    int new_socket = *(int*)socket;
    buffer[0] = '\0';
	int x = read(new_socket, buffer,1024);
	printf("%s is the read data by root\n",buffer);
		    
	present = search(hash_root,buffer);
           
    //cout << present << "\n after search results\n";
    if (present)
       dest_port = set_port(buffer); 
	
    else if(!present)
       dest_port = 0; 	 


    snprintf (send_buffer, sizeof(send_buffer), "%d",dest_port);
    puts(send_buffer);
    send(new_socket,send_buffer,1024, 0);
    close(new_socket);


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
    if (listen(server_fd, 50) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    insert_in_hash(hash_root, file);

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
            printf("\nroot server data.........\n");
	        new_sock = (int *)malloc(sizeof *new_sock);
            *new_sock = new_socket;
            rc=pthread_create(&pth,NULL,threadFunc,(void *)new_sock);	   
        }
	    
	}

     return 0;
}