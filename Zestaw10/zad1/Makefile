CC=gcc
M=Makefile
P1=server
P2=client

compile:
	make clear
	$(CC) $(P1).c -pthread -o $(P1)
	$(CC) $(P2).c -pthread -o $(P2)

clear:
	 find . ! -name $(P1).c ! -name $(P2).c ! -name $(M) -exec rm {} \;
