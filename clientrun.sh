g++ -c -std=c++11 client.cpp
g++ -c -std=c++11 connection.cpp
g++ -c -std=c++11 hashmap.cpp
g++ hashmap.o connection.o client.o -pthread -o client
./client

