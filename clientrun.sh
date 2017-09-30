g++ -c -std=c++11 $1.cpp
g++ -c -std=c++11 connection.cpp
g++ -c -std=c++11 hashmap.cpp
g++ hashmap.o connection.o $1.o -pthread -o $1


