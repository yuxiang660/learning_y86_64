CC=gcc
CFLAGS=-Wall -g
# http://dinosaur.compilertools.net/flex/manpage.html
LEX=flex

LEX_TARGET=yas-grammar
ASSEMBLER=yas

SRCS := yas.c isa.c
OBJS := $(LEX_TARGET).o $(SRCS:.c=.o)

all: clean $(ASSEMBLER)

$(LEX_TARGET).c: $(LEX_TARGET).lex
	$(LEX) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

$(ASSEMBLER): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

run:
	./$(ASSEMBLER) example.ys
	cat example.yo

clean:
	rm -rf $(ASSEMBLER) $(LEX_TARGET).c *.o *.yo

yis.o: yis.c isa.h
	$(CC) $(CFLAGS) -c yis.c

yis: yis.o isa.o
	$(CC) $(CFLAGS) yis.o isa.o -o yis

run_yis:
	./yas asum.ys
	./yis asum.yo
