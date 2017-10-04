#include <bits/stdc++.h>
#include <map>
#include "connection.h"
#include "hashmap.h"

using namespace std;



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
        cout << "\n Socket creation error \n";
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "\nConnection Failed \n";
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

    //cout << buffer <<  " turn client mode on" << endl;

    for(i = (strlen(buffer)-1) ; i>0 ; i--)
    	if(buffer[i] == '.') break;
    //cout << i << " is index before com\n";

    final_buffer[0] = 'w';final_buffer[1] = 'w'; final_buffer[2] = 'w';
    for(j = 3; buffer[i] != '\0' ; j++,i++ )
    final_buffer[j] = buffer[i];

	final_buffer[j] = '\0';

    //cout << final_buffer << " turn client mode on";
    send(sock,final_buffer ,100, 0 );
    while(read(sock,read_buffer,1024)<0);
    //printf("%s\n",read_buffer );
    //cout << read_buffer <<  " send by rootserver\n";
    int Dest_port = atoi(read_buffer);
    //turnon_client_mode(Dest_port);
    close(sock);
    //shutdown(sock,SHUT_RDWR);
    
    return Dest_port;
}








char * turn_clientmode_on(char *buffer, int port)
{


	int i, j = 0, j1 = 0, count = 0;
	char * final_buffer = (char*) malloc(10*sizeof(char));
    
    char * read_buffer = (char*) malloc(1024*sizeof(char));
   
    int sock = client_initialise(port);

    int dot[10], dot_index = 0;


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

         		 printf("\nsend failed in resolver while sending to other server\n");
         		 //return NULL;

        }


        read(sock,read_buffer,1024);


    }
    
    close(sock);
    //shutdown(sock, SHUT_RDWR);
    
    return read_buffer;
}
