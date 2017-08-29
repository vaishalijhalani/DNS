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
#include "trie.h"
#include "connection.h"
#define PORT 8080
#define ROOT_PORT 8081


struct dnsnode* root;

pthread_mutex_t lock;

void * threadFunc(void * socket)
{  
		int Dest_port;
		int new_socket = *(int*)socket;
		char buffer[1024] = {0};
		char * send_buffer = (char * )malloc(1024*sizeof(char));
	    char * present = (char*) malloc(1024*sizeof(char));
	    char * url = (char*) malloc(1024*sizeof(char));
		int x = read(new_socket, buffer,1024);
		printf("\n\n\n %s is read by server\n", buffer);
		present = search(root,buffer);

		if(!present)
		{
			   printf("\n not present in server cache\n");
			   //strcpy(url,buffer);
			   Dest_port = turn_clientmode_on_for_root(buffer, ROOT_PORT);
			   printf("%d before send\n",Dest_port);

			   if(Dest_port!= -1)
	  		   {
	  				
				   		present = turn_clientmode_on(buffer, Dest_port);
				   		//printf("hello");
				   		//printf("%s after turn on..............................", present);
			

						if(!strcmp(present,"N"))
						{
								//sprintf("\nsending the null value \n");
								send(new_socket, present, 1024,0);
						}
							
			            //printf("\n %s is return by com or in server\n",present);

						else
						{
								pthread_mutex_lock(&lock);
								printf("\ninsernt in tree\n");
						   		Insertdata(root, url, present );
						   		pthread_mutex_unlock(&lock);
						   		send(new_socket, present, 1024,0);
						}
			   
				}

				else 
				{
				
						present = "D";
		                //printf("%s after search results in .in server\n",present);
		                send(new_socket,present,1024, 0);
				}		

			}
	else
	{
			fflush(stdout);
		    //printf("IP returned from server cache");
			send(new_socket, present, 1024, 0 );
			printf("\n IP returned from server cache\n\n\n\n\n\n");
	}
}

int main(int argc, char const *argv[])
{


	
	//char * root_buffer = (char*) malloc(1024*sizeof(char));
	//int i = 0, j = 0;
	int new_socket,*new_sock;
	root  = newnode();
	pthread_t pth;
    int rc; 
	insert_in_tree(root,"insert.txt");
	int server_fd, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);


	if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

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
	   
		if(new_socket > 0 )
		{

				new_sock = malloc(sizeof *new_sock);
	            *new_sock = new_socket;
	            rc=pthread_create(&pth,NULL,threadFunc,(void *)new_sock);    
	        
	
			
		}
	}

	pthread_exit(NULL);
	return 0;
}