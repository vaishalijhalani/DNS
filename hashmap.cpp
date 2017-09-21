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
#include <string>
#include <unordered_map>
#include <typeinfo> 
#include "connection.h"
#include "hashmap.h"

using namespace std;


void insert_in_hash(unordered_map <std::string,std::string> &hashmap, const char * file)
{

   
	char * url_insert, * ip_insert;

	unordered_map<std::string,std::string>::iterator it;

	char line[50];

	url_insert = (char*)malloc(50 * sizeof(char));

	ip_insert = (char*)malloc(50 * sizeof(char));

 	FILE * file1 = fopen ( file, "r" );

 	int i, j, k, length;

	while(fgets ( line, sizeof(line), file1)!= NULL ) 
    {

		i = 0;

		j = 0;

		k = 0;

		length = strlen(line);

		url_insert[0] = '\0';

		ip_insert[0] = '\0';//initialise all the aaray ip_insert and url_insert
		
		while(line[i] != ' ') url_insert[k++] = line[i++];

		k = i;

		i++;

		while(i < length) ip_insert[j++] = line[i++];

		ip_insert[j] = '\0';

		url_insert[k] = '\0';

		//printf("\n %s and %s are inserted pairwise\n",url_insert,ip_insert);

		std::string url_insert1(url_insert);

		std::string ip_insert1(ip_insert);

		//cout << typeid(url_insert1).name() << typeid(ip_insert1).name();

		if(hashmap.insert(std::make_pair(url_insert1, ip_insert1)).second == false)
			printf("insertion failed\n");

		it = hashmap.begin();
		//cout << "hash key" << it->first;


	}

}



string search (unordered_map <std::string, std::string> hashmap, std::string url)
{


	if(hashmap.find(url) == hashmap.end())
	{
		printf("data not present\n");
		return NULL;
	}
		

	else 
	{
		printf("data searched\n");

		return hashmap.find(url) -> second;
	}

}
