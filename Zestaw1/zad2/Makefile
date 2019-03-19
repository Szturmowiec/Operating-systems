compile_and_test:
	make clear
	gcc -c tablib.c
	ar rcs tablib.a tablib.o
	gcc -c main.c
	gcc main.o tablib.a -o main
	./main 10000 999 dynamic create_table add 10000 search_element 1 >>raport2.txt
	./main 10000 999 dynamic create_table add 10000 add_and_remove 10000 >>raport2.txt
	./main 1000 999 static create_table add 1000 search_element 1 >>raport2.txt
	./main 1000 999 static create_table add 1000 add_and_remove 10000 >>raport2.txt

compile:
	make clear
	gcc -c tablib.c
	ar rcs tablib.a tablib.o
	gcc -c main.c
	gcc main.o tablib.a -o main

clear:
	rm -f *.o *.a main raport2.txt
