CC=gcc
M=Makefile
P1=master
P2=slave
F="potok"
D="date.sh"

compile:
	make clear
	$(CC) $(P1).c -o $(P1)
	$(CC) $(P2).c -o $(P2)

test:
	make compile
	./$(P1) $(F) &
	./$(P2) $(F) 5 &
	./$(P2) $(F) 2 &
	./$(P2) $(F) 1 &
	./$(P2) $(F) 4 &
	./$(P2) $(F) 3 &

clear:
	 find . ! -name $(P1).c ! -name $(P2).c ! -name $(M) ! -name $(D) -exec rm {} \;