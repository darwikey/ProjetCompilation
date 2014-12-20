LEX=lex
YACC=bison
CFLAGS=-Wall
CC=gcc
CCP=g++

all: parser

parser:parser.cpp scanner.o
	$(CCP) $(CFLAGS) -o $@ $^

parser.cpp:parser.y
	$(YACC) -o $@ --defines=parser.tab.h $^

scanner.o:scanner.c
	$(CC) $(CFLAGS) -c $^ -o $@

scanner.c:scanner.l
	$(LEX) -o $@ $^
tests:
	for i in `ls tests/*.c`; do echo $$i; ./parser $$i; done

clean:
	rm -f parser.cpp parser.tab.h parser.tab.c scanner.c *.o
