CC=gcc
M=Makefile
P1=server
P2=client
Z=zlecenia

compile:
	make clear
	$(CC) $(P1).c -lrt -o $(P1)
	$(CC) $(P2).c -lrt -o $(P2)

clear:
	 find . ! -name $(P1).c ! -name $(P2).c ! -name $(M) ! -name $(Z) -exec rm {} \;
