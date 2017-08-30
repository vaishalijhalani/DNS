gcc -c $1.c
gcc -c connection.c
gcc -c trie.c
gcc trie.o connection.o $1.o -pthread -o $1
./$1

