#include <bits/stdc++.h>
#include <tr1/unordered_map>
using namespace std::tr1;
#ifndef _hashmap_h
#define _hashmap_h

char * search (unordered_map<char *, char *> hashmap, char *url);  //Search in dns cache

void insert_in_hash (unordered_map<char *, char *> hashmap, const char * file); //insert data in hash

#endif