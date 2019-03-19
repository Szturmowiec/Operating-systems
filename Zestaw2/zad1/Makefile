CC=gcc
P=wewy
M=Makefile
P1=dane
P2=dane_copy
S=sys
L=lib
G=generate
C=copy
SR=sort

compile:
	make clear
	$(CC) $(P).c -o $(P)

test_full:
	make compile
	make test4
	rm $(P1) $(P2)
	make test512
	rm $(P1) $(P2)
	make test4096
	rm $(P1) $(P2)
	make test8192

test4:
	./$(P) $(G) $(P1) 50000 4
	./$(P) $(C) $(P1) $(P2) 50000 4 $(S) >>wyniki.txt
	./$(P) $(C) $(P1) $(P2) 50000 4 $(L) >>wyniki.txt
	./$(P) $(SR) $(P1) 50000 4 $(S) >>wyniki.txt
	./$(P) $(SR) $(P2) 50000 4 $(L) >>wyniki.txt

test512:
	./$(P) $(G) $(P1) 20000 512
	./$(P) $(C) $(P1) $(P2) 20000 512 $(S) >>wyniki.txt
	./$(P) $(C) $(P1) $(P2) 20000 512 $(L) >>wyniki.txt
	./$(P) $(SR) $(P1) 20000 512 $(S) >>wyniki.txt
	./$(P) $(SR) $(P2) 20000 512 $(L) >>wyniki.txt

test4096:
	./$(P) $(G) $(P1) 10000 4096
	./$(P) $(C) $(P1) $(P2) 10000 4096 $(S) >>wyniki.txt
	./$(P) $(C) $(P1) $(P2) 10000 4096 $(L) >>wyniki.txt
	./$(P) $(SR) $(P1) 10000 4096 $(S) >>wyniki.txt
	./$(P) $(SR) $(P2) 10000 4096 $(L) >>wyniki.txt

test8192:
	./$(P) $(G) $(P1) 10000 8192
	./$(P) $(C) $(P1) $(P2) 10000 8192 $(S) >>wyniki.txt
	./$(P) $(C) $(P1) $(P2) 10000 8192 $(L) >>wyniki.txt
	./$(P) $(SR) $(P1) 10000 8192 $(S) >>wyniki.txt
	./$(P) $(SR) $(P2) 10000 8192 $(L) >>wyniki.txt

clear:
	 find . ! -name $(P).c ! -name $(M) -exec rm {} \;
