#include <bits/stdc++.h>
#include <tr1/unordered_map>
#include "connection.h"
#include "hashmap.h"

using namespace std::tr1;

int errno;
#define PORT 8080
#define ROOT_PORT 8081

unordered_map<char * , char *> hash_server;

void * threadFunc(void * socket)
{  
		int Dest_port;
		int new_socket = *(int*)socket;
		char buffer[1024] = {0}, send_buffer[1024] = {0};
	    const char * present = (char*) malloc(1024*sizeof(char));
	    char * url = (char*) malloc(1024*sizeof(char));
		int x = read(new_socket, buffer,1024);
			std::cout << buffer <<  " is read by server...............................................\n";
			present = search(hash_server,buffer);
			if(!present)
			{
			   printf("not present in server cache\n");
			   strcpy(url,buffer);
			   Dest_port = turn_clientmode_on_for_root(buffer, ROOT_PORT);
			   //cout << Dest_port << " before send\n";

				if(Dest_port != 0)
	  			{
		     	   
                    present = turn_clientmode_on(buffer, Dest_port);
		
			         //cout << present <<  " is return by com or in server\n";
			        if(present != "N")
			   		hash_server.insert(std::make_pair(url, present));;
			        if(send(new_socket,present,1024, 0)==-1)
                    {
         		 			printf("\nsend failed in resolver\n");
         		 
         		    }
				
                }

				else if (Dest_port == 0)
				{
				    present = "D";
				    //puts(present);
                 	sleep(0.1);
                    //cout << "\nnot a valid domain\n";
                    
                    int temp = write(new_socket,present,1024);
                    if(temp==-1)
                    {
         		 			printf("\nsend failed in resolver\n");
                    		strerror(errno);
         		 			// cout << "an error: << strerror(errno) <<  "\n";

         		 
         		    }
                 
                    //cout << "\nafter send\n";
				}		

			}
			else
			{
				fflush(stdout);

			    //cout << "\n IP returned from server cache";
				send(new_socket, present, 1024, 0 );
				printf("IP returned from server cache\n");
			}

			close(new_socket);
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
