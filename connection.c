
#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include "connection.h"



int set_port(char * buffer)
{
	int i,j;
	char * x = (char*) malloc(10*sizeof(char));
	for(i=0,j=4;buffer[j]!='\0';j++,i++)
		x[i] = buffer[j];
	   
    x[i] = '\0';

	if(strcmp(x,"in") == 0)
		return 9091;
    else if(strcmp(x, "com") == 0)
        return 9092;
    else 
        return 0;
}


int  client_initialise(int port)
{
	struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;
        
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    return sock;

}





int turn_clientmode_on_for_root(char *buffer, int port)
{


    int sock = client_initialise(port);
    char * final_buffer = (char*) malloc(10*sizeof(char));
    
    char * read_buffer = (char*) malloc(1024*sizeof(char));
    int i , j;

    //printf("%s turn client mode on",buffer);

    for(i = (strlen(buffer)-1) ; i>0 ; i--)
    	if(buffer[i] == '.') break;
    //printf("%d is index before com\n", i);

    final_buffer[0] = 'w';final_buffer[1] = 'w'; final_buffer[2] = 'w';
    for(j = 3; buffer[i] != '\0' ; j++,i++ )
    final_buffer[j] = buffer[i];

	final_buffer[j] = '\0';

    //printf("%s turn client mode on",final_buffer);
    send(sock,final_buffer ,100, 0 );
    while(read(sock,read_buffer,1024)<0);
    printf("%s\n",read_buffer );
    //printf("%s send by rootserver\n",read_buffer);
    int Dest_port = atoi(read_buffer);
    //turnon_client_mode(Dest_port);
    close(sock);
    return Dest_port;
}








char * turn_clientmode_on(char *buffer, int port)
{


	int i, j = 0, j1 = 0, count = 0;
	char * final_buffer = (char*) malloc(10*sizeof(char));
    
    char * read_buffer = (char*) malloc(1024*sizeof(char));
   
    int sock = client_initialise(port);

    int dot[10], dot_index = 0;
    //printf("%s turn client mode on",buffer);

    for(i = (strlen(buffer)-1) ; i>0 ; i--)
    {
    	if(buffer[i] == '.') 
    		{
    		      dot[j++] = i;	
    		
    		}
    }

    dot_index = j ;
    

    snprintf (final_buffer, sizeof(final_buffer), "%d",dot_index);

    send(sock, final_buffer ,1024, 0 );
    
    final_buffer[0] = '\0';
    //for(i=0;i<dot_index;i++) printf("%d\n", dot[i]);

    final_buffer[0] = 'w';final_buffer[1] = 'w'; final_buffer[2] = 'w';
    for(j1=0; j1<dot_index ; j1++)
    {
        i = dot[j1];

        for(j = 3; buffer[i] != '\0' ; j++,i++ )
        {
            final_buffer[j] = buffer[i];
        }

        final_buffer[j] = '\0';
        //printf("\n%s turn client mode on\n",final_buffer);

        if(send(sock,final_buffer ,1024, 0 )==-1)
            {

         		 printf("\nsend failed in resolver\n");
         		 //return NULL;

         	}

        //printf("data sent");
        read(sock,read_buffer,1024);
        //printf("%s send by inserver\n",read_buffer);

    }
    
    close(sock);
    return read_buffer;
}
