#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#ifndef _trie_h
#define _trie_h

struct dnsnode
{
	struct dnsnode *alphabets[27];
	char * ipaddress;
	bool flag;

};

struct dnsnode * newnode(); //allocate the memory to a new node

void Insertdata (struct dnsnode * root, char * url,char *ip);  //Insertion in dns cache

char * search (struct dnsnode * root, char *url);  //Search in dns cache

void insert_in_tree(struct dnsnode * root, char * file); //insert data in tree

#endif