run: program.c
	gcc -o program program.c -I/usr/include/libxml2 -lxml2
	./program