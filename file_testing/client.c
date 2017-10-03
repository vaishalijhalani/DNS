// Client side C/C++ program to demonstrate Socket programming
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
  
int main(int argc, char const *argv[])
{

    clock_t startTime = clock();
    float start = ((float)startTime)/CLOCKS_PER_SEC;
 
   while(1){

    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
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

 
        send(sock , hello , strlen(hello) , 0 );
        valread = read( sock , buffer, 1024);
        float secsElapsed = ((float)clock())/CLOCKS_PER_SEC - start;
        printf("%f %s\n",secsElapsed, buffer);
        if(secsElapsed > 300 )
        {
            printf("%f %s\n",secsElapsed, buffer); 
            break;
        }


	close(sock);

    }
    
    return 0;
}
