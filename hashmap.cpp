#include <bits/stdc++.h>
#include <tr1/unordered_map>
#include "connection.h"
#include "hashmap.h"

using namespace std::tr1;


void insert_in_hash(unordered_map <char *, char*> hashmap, const char * file)
{


	char * url_insert, * ip_insert;

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

		ip_insert[0] = '\0';
		
		while(line[i] != ' ') url_insert[k++] = line[i++];

		k = i;

		i++;

		while(i < length) ip_insert[j++] = line[i++];

		ip_insert[j] = '\0';

		url_insert[k] = '\0';

		hashmap.insert(std::make_pair(url_insert, ip_insert));

	}

}



char * search (unordered_map <char*, char *> hashmap, char *url)
{

	if(hashmap.find(url) == hashmap.end())

		return NULL;

	else return hashmap.find(url) -> second;

}
