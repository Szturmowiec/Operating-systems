static:
	make clear
	gcc -c tablib.c
	ar rcs tablib.a tablib.o

shared:
	make clear
	gcc -c -Wall -fPIC tablib.c
	gcc -shared -fPIC -o tablib.so tablib.o

clear:
	rm -f *.o *.a *.so