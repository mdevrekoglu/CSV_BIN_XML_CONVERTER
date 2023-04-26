run: program.c
	gcc -o program program.c -I/usr/include/libxml2 -lxml2 -lm
	./program records.csv binary.dat 1
	./program binary.dat records.xml 2
	./program records.xml records.xsd 3
	