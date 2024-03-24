CC = cc

make_linux:
	$(CC) -c ./src/powerbuffer.c -o powerbuffer.o
	ar rcs libpowerb.a powerbuffer.o 
	rm -rf ./powerbuffer.o 
