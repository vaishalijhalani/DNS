#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdbool.h>
#include <unordered_map>
using namespace std;
#ifndef _hashmap_h
#define _hashmap_h

std::string search (unordered_map<std::string, std::string> hashmap, std::string url);  //Search in dns cache

void insert_in_hash (unordered_map<std::string, std::string> &hashmap, const char * file); //insert data in hash

#endif