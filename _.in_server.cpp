#include <bits/stdc++.h>
#include <tr1/unordered_map>
#include "connection.h"
#include "hashmap.h"

using namespace std::tr1;
#define PORT 9091


unordered_map<char * , char *> hash_in;

void * threadFunc(void * socket)
{  
	char buffer[1024] = {0}, send_buffer[1024] = {0};
    const char * present = (char*) malloc(1024*sizeof(char));
    int new_socket = *(int*)socket;
 	read(new_socket, buffer,1024);
    int iterate = atoi(buffer);
 
    for(int i = 0 ; i < iterate ; i++)
     {
                 //cout << "in the loop";
                 //memset(buffer,'\0',sizeof(buffer));
                 //cout << "before reading buffer";
        int x = read(new_socket, buffer,1024);
                 //cout << buffer << endl;
         present = search(hash_in,buffer);
         if(!present)
       	    present = "N";
                 
                 //cout << present << " after search results in .in server\n";
        send(new_socket,present,1024, 0);
                 //memset(buffer,'\0',sizeof(buffer));
            }   

    close(new_socket); 

}

int main(int argc, char const *argv[])
{
    
    const char * file = (char*) malloc(1024*sizeof(char));
    file = "insert_in.txt";

    insert_in_hash(hash_in,file);

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
    if (listen(server_fd, 50) < 0)
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
            //memset(buffer,'\0',sizeof(buffer));
           
        	new_sock = (int*)malloc(sizeof *new_sock);
            *new_sock = new_socket;
            rc=pthread_create(&pth,NULL,threadFunc,(void *)new_sock);  
        }

    }

    return 0;

}

