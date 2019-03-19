CC=gcc
M=Makefile
P=filter
F1=fil
F2=fil2
T1=test
T2=test2
T=Times.txt
W=wynik

compile:
	make clear
	$(CC) $(P).c -pthread -lm -o $(P)

all:
	make compile
	./$(P) 1 $(T1) $(F1) $(W)>$(T)
	./$(P) 1 $(T1) $(F2) $(W)>>$(T)
	./$(P) 2 $(T1) $(F1) $(W)>>$(T)
	./$(P) 2 $(T1) $(F2) $(W)>>$(T)
	./$(P) 4 $(T1) $(F1) $(W)>>$(T)
	./$(P) 4 $(T1) $(F2) $(W)>>$(T)
	./$(P) 8 $(T1) $(F1) $(W)>>$(T)
	./$(P) 8 $(T1) $(F2) $(W)>>$(T)

clear:
	 find . ! -name $(P).c ! -name $(F1) ! -name $(F2) ! -name $(M) ! -name $(T) ! -name $(T1) ! -name $(T2) -exec rm {} \;
