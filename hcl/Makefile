CC=gcc
CFLAGS=-Wall -O1 -g
# http://dinosaur.compilertools.net/flex/manpage.html
LEX=flex
YACC=bison

LEX_SRC=hcl.lex
LEX_OUT=hcl_lex.c

YACC_SRC=hcl.y
YACC_OUT=hcl.tab.c

HCL2C=hcl2c
HCL2V=hcl2v

SRCS := $(LEX_OUT) $(YACC_OUT) node.c outgen.c

all: $(HCL2C) $(HCL2V)

$(LEX_OUT): $(LEX_SRC)
	$(LEX) -o $@ $<

$(YACC_OUT): $(YACC_SRC)
	$(YACC) -d $<

$(HCL2C): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

$(HCL2V): $(SRCS)
	$(CC) $(CFLAGS) -DVLOG $^ -o $@

run:
	./$(HCL2C) < example.hcl

mux4:
	./$(HCL2C) < example.hcl > mux4.c
	$(CC) $(CFLAGS) mux4.c -o mux4

seq:
	./$(HCL2C) < seq_std.hcl > seq_std.c

seq_v:
	./$(HCL2V) < seq_std.hcl

clean:
	rm -rf $(HCL2C) $(HCL2V) $(LEX_OUT) $(YACC_OUT) hcl.tab.h mux4* seq_std.c
