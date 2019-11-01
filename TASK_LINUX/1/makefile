all:copyfile

copyfile: main.o CopyFile.o
	gcc main.o CopyFile.o -o copyfile

main.o: main.c
	gcc -c main.c

CopyFile.o: CopyFile.c
	gcc -c CopyFile.c

clean:
	rm -rf *.o copyfile

	

