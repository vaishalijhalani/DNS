gcc -c client.c
gcc -c connection.c
gcc -c trie.c
gcc trie.o connection.o client.o -pthread -o client
./client

