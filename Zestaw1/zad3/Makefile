all:
	make clear
	make compile_and_test_static
	make compile_and_test_static_o0
	make compile_and_test_static_o1
	make compile_and_test_static_o2
	make compile_and_test_static_os

compile_and_test_static:
	make clear
	gcc -c tablib.c
	ar rcs tablib.a tablib.o
	gcc -c main.c
	gcc main.o tablib.a -o main
	./main 10000 999 dynamic create_table add 10000 search_element 1 >>results3a.txt
	./main 10000 999 dynamic create_table add 10000 add_and_remove 10000 >>results3a.txt
	./main 1000 999 static create_table add 1000 search_element 1 >>results3a.txt
	./main 1000 999 static create_table add 1000 add_and_remove 10000 >>results3a.txt

compile_and_test_static_o0:
	make clear
	gcc -O0 -c tablib.c
	ar rcs tablib.a tablib.o
	gcc -O0 -c main.c
	gcc -O0 main.o tablib.a -o main
	./main 10000 999 dynamic create_table add 10000 search_element 1 >>results3b.txt
	./main 10000 999 dynamic create_table add 10000 add_and_remove 10000 >>results3b.txt
	./main 1000 999 static create_table add 1000 search_element 1 >>results3b.txt
	./main 1000 999 static create_table add 1000 add_and_remove 10000 >>results3b.txt

compile_and_test_static_o1:
	make clear
	gcc -O1 -c tablib.c
	ar rcs tablib.a tablib.o
	gcc -O1 -c main.c
	gcc -O1 main.o tablib.a -o main
	./main 10000 999 dynamic create_table add 10000 search_element 1 >>results3b.txt
	./main 10000 999 dynamic create_table add 10000 add_and_remove 10000 >>results3b.txt

compile_and_test_static_o2:
	make clear
	gcc -O2 -c tablib.c
	ar rcs tablib.a tablib.o
	gcc -O2 -c main.c
	gcc -O2 main.o tablib.a -o main
	./main 10000 999 dynamic create_table add 10000 search_element 1 >>results3b.txt
	./main 10000 999 dynamic create_table add 10000 add_and_remove 10000 >>results3b.txt

compile_and_test_static_os:
	make clear
	gcc -Os -c tablib.c
	ar rcs tablib.a tablib.o
	gcc -Os -c main.c
	gcc -Os main.o tablib.a -o main
	./main 10000 999 dynamic create_table add 10000 search_element 1 >>results3b.txt
	./main 10000 999 dynamic create_table add 10000 add_and_remove 10000 >>results3b.txt

clear:
	rm -f *.o *.a main