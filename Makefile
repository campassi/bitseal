all:
	gcc -std=c11 -lcrypto -Wextra -Wall -pedantic bitseal.c -o bitseal
