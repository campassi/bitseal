all:
	gcc bitseal.c -o bitseal -std=c11 -lcrypto -Wextra -Wall -pedantic
