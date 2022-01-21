CC=g++
CFLAGS=-Wall -O1 -g
LEX=flex
LEXLIB=-lfl

LEX_TARGET=yas-grammar
ASSEMBLER=yas

SRCS := yas.c isa.c
OBJS := $(LEX_TARGET).o $(SRCS:.c=.o)

all: $(ASSEMBLER)

$(LEX_TARGET).c: $(LEX_TARGET).lex
	$(LEX) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

$(ASSEMBLER): $(OBJS)
	$(CC) $(CFLAGS) $^ ${LEXLIB} -o $@

run:
	./$(ASSEMBLER) example.ys
	cat example.yo

clean:
	rm -rf $(ASSEMBLER) yas-grammar.c *.o *.yo
