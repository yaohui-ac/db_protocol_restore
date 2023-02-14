main:main.o
	gcc  main.o -o main -lpcap
main.o:main.c
	gcc -c main.c -o main.o 
	
