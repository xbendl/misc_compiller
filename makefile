# Makefile

#parametry prekladace C
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -g

all: ifj07

ifj07: avl.o compiler.o expr.o interpret.o keyword_table.o mergesort.o parser.o scanner.o str.o tape.o test.o
	$(CC) $(CFLAGS) -lm avl.o compiler.o expr.o interpret.o keyword_table.o mergesort.o parser.o scanner.o str.o tape.o test.o -o ifj07

avl.o: avl.c avl.h
	$(CC) $(CFLAGS) -c avl.c -o avl.o
	
compiler.o: compiler.c
	$(CC) $(CFLAGS) -c compiler.c -o compiler.o
	
expr.o: expr.c expr.h
	$(CC) $(CFLAGS) -c expr.c -o expr.o

interpret.o: interpret.c interpret.h
	$(CC) $(CFLAGS) -c interpret.c -o interpret.o
	
keyword_table.o: keyword_table.c keyword_table.h
	$(CC) $(CFLAGS) -c keyword_table.c -o keyword_table.o
	
mergesort.o: mergesort.c mergesort.h
	$(CC) $(CFLAGS) -c mergesort.c -o mergesort.o
	
parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o
	
scanner.o: scanner.c scanner.h
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o
	
str.o: str.c str.h
	$(CC) $(CFLAGS) -c str.c -o str.o
	
tape.o: tape.c tape.h
	$(CC) $(CFLAGS) -c tape.c -o tape.o

test.o: test.c test.h
	$(CC) $(CFLAGS) -c test.c -o test.o

clean-o:
	rm -f *.o

clean: clean-o