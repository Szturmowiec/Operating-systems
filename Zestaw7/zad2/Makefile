CC=gcc
M=Makefile
P1=goli
P2=klient

compile:
	make clear
	$(CC) $(P1).c -pthread -lrt -o $(P1)
	$(CC) $(P2).c -pthread -lrt -o $(P2)

clear:
	 find . ! -name $(P1).c ! -name $(P2).c ! -name $(M) -exec rm {} \;
