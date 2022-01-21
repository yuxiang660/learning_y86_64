CC=g++
CFLAGS=-Wall -O1 -g
LEX = flex
LEXLIB = -lfl
YAS=./yas

all: yas

yas-grammar.c: yas-grammar.lex
	$(LEX) yas-grammar.lex
	mv lex.yy.c yas-grammar.c

yas-grammar.o: yas-grammar.c
	$(CC) $(CFLAGS) -c yas-grammar.c

yas.o: yas.c yas.h isa.h
	$(CC) $(CFLAGS) -c yas.c

isa.o: isa.c isa.h
	$(CC) $(CFLAGS) -c isa.c

yas: yas.o yas-grammar.o isa.o
	$(CC) $(CFLAGS) yas-grammar.o yas.o isa.o ${LEXLIB} -o yas

run:
	$(YAS) example.ys
	cat example.yo

clean:
	rm -rf yas-grammar.c
