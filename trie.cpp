#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string.h>
#include <stdbool.h>
#include "trie.h" 


struct dnsnode * newnode()
{

	struct dnsnode * new = (struct dnsnode *) malloc(sizeof (struct dnsnode));
	int i = 0;
	for(i=0;i<27;i++) 
		new->alphabets[i] =  NULL;
	return new;
}


void Insertdata (struct dnsnode * root, char * url,char *ip)
{
	int length = strlen(url);
	int i, index;
	struct dnsnode * temp = root;


	for(i=4;i<length;i++)
	{

	    if(url[i] != '.')

			index = url[i]-'a';

		else if (url[i] == '.')

			index = 27;

		if(!(temp->alphabets[index]))
		{			
			temp->alphabets[index] = newnode();
			temp = temp->alphabets[index];

		}

		else temp = temp->alphabets[index];

        


	}

	temp->flag = 1;
	temp->ipaddress = (char*)malloc(length * sizeof(char));
	strcpy(temp->ipaddress,ip);

}

char* search (struct dnsnode * root, char *url)
{

	int length = strlen(url);
	bool f;
	struct dnsnode * temp = root;
	int index;
	//printf("%d\n",length);
	for(int i=4;i<length;i++)
	{

	 if(url[i] != '.')

			index = url[i]-'a';//printf("\n value of index..............%d\n",index );}

		else if (url[i] == '.')

			index = 27;

		
		if((temp->alphabets[index]))
		{
			temp = temp->alphabets[index];
			printf("index for url %d %c\n", index,url[i]);
			f = 1;
		}

		else if(!(temp->alphabets[index])) { f = 0;  break;}

        

	}

	printf("searching data\n");
	//printf("%s\n",temp->ipaddress );
	if(temp->flag && f) return temp->ipaddress;
	else return NULL;

}

	
void insert_in_tree(struct dnsnode * root, char * file)
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

		ip_insert[0] = '\0';//initialise all the aaray ip_insert and url_insert
		
		while(line[i] != ' ') url_insert[k++] = line[i++];

		k = i;

		i++;

		while(i < length) ip_insert[j++] = line[i++];

		ip_insert[j] = '\0';

		url_insert[k] = '\0';
		
		//printf("%s %s\n",url_insert, ip_insert);
	        Insertdata(root,url_insert,ip_insert);
        }

	fclose(file1);

}
