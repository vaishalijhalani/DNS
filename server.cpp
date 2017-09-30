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


int errno;
#define PORT 8080
#define ROOT_PORT 8081

pthread_mutex_t lock;

unordered_map<std::string, std::string> hash_server;

void * threadFunc(void * socket)
{  
		int Dest_port;
		int new_socket = *(int*)socket;
		char * buffer = (char*) malloc(1024*sizeof(char));
		char * send_to_client = (char*) malloc(1024*sizeof(char));
		std::string present(1024,0);
	    //buffer.reserve(1024);
	    //send_buffer.reserve(1024);
	    present.reserve(1024);

	    clock_t startTime = clock();
		int x = read(new_socket,buffer,1024);
		//std::cout << buffer << x << " " << " is read by server...............................................\n";
		std::string send_buffer(buffer);
		present = search(hash_server,send_buffer);


		if(present.empty())
		{

			   //printf("not present in server cache\n");
			   Dest_port = turn_clientmode_on_for_root(buffer, ROOT_PORT);
			   //cout << Dest_port << " before send\n";

				if(Dest_port != 0)
	  			{
		     	   
                    present = turn_clientmode_on(buffer, Dest_port);
		
			         //cout << present <<  " is return by com or in server\n";
			        	
			        send(new_socket,present.c_str(),1024, 0);
					float secsElapsed = (float)(clock() - startTime)/CLOCKS_PER_SEC;
    				std::cout << secsElapsed*1000 << endl;
				
                }

				else if (Dest_port == 0)
				{
				    present = "D";
				    
                 	//sleep(0.1);
                    //cout << "\nnot a valid domain\n";
                    
                    write(new_socket,present.c_str(),1024);
                    float secsElapsed1 = (float)(clock() - startTime)/CLOCKS_PER_SEC;
    				std::cout << secsElapsed1*1000 << endl;

                    //cout << "\nafter send\n";
				}		

		}
			else
			{
				

			    //std::cout << "\n IP returned from server cache";
			    send_to_client = &present[0]; 
				send(new_socket,send_to_client , 1024, 0 );
				float secsElapsed2 = (float)(clock() - startTime)/CLOCKS_PER_SEC;
    			std::cout << secsElapsed2*1000 << endl;
				//printf("IP returned from server cache\n");
			}

			close(new_socket);
			//shutdown(new_socket,SHUT_RDWR);
			
			//float secsElapsed2 = (float)(clock() - startTime)/CLOCKS_PER_SEC;
    		//std::cout << secsElapsed2*1000 << endl;
}

int main(int argc, char const *argv[])
{


	
	//char * root_buffer = (char*) malloc(1024*sizeof(char));
	//int i = 0, j = 0;
	int new_socket,*new_sock;
	const char * file = (char*) malloc(1024*sizeof(char));
    file = "insert.txt";
	pthread_t pth;
    int rc; 
	insert_in_hash(hash_server,file);

	//cout << "values in hashmap\n";
	//for(unordered_map<std::string,std::string>::iterator it = hash_server.begin(); it != hash_server.end(); ++it) {
 
        //cout <<"value in hash\n" << it->first << " " << it->second;}

	int server_fd, valread;
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
	if (listen(server_fd, 100000) < 0)
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
            std::cout << "\ncreating new thread....\n";
			new_sock = (int*)malloc(sizeof *new_sock);
            *new_sock = new_socket;
            rc=pthread_create(&pth,NULL,threadFunc,(void *)new_sock);    
        
	
			
		}
}

	pthread_exit(NULL);
	return 0;
}
