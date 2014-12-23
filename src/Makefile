.PHONY: all parser tests clean

LEX=lex
YACC=bison
CFLAGS=-Wall -Wno-write-strings -std=c++11
CCP=g++

all: parser

parser:parser.cpp scanner.cpp Block.cpp Main_block.cpp
	$(CCP) $(CFLAGS) -o $@ $^

parser.cpp:parser.y
	$(YACC) -o $@ --defines=parser.tab.h $^

scanner.cpp:scanner.l
	$(LEX) -o $@ $^
tests:
	for i in `ls tests/*.c`; do echo $$i; ./parser $$i; done

clean:
	rm -f parser.cpp parser.tab.h parser.tab.c scanner.cpp
