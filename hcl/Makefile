CC=gcc
CFLAGS=-Wall -O1 -g
# http://dinosaur.compilertools.net/flex/manpage.html
LEX=flex
YACC=bison

LEX_SRC=hcl.lex
LEX_OUT=hcl_lex

YACC_SRC=hcl.y
YACC_OUT=hcl.tab

HCL2C=hcl2c
HCL2V=hcl2v

SRCS := $(LEX_OUT).c $(YACC_OUT).c node.c outgen.c

all: $(HCL2C) $(HCL2V)

$(LEX_OUT).c: $(LEX_SRC)
	$(LEX) -o $@ $<

$(YACC_OUT).c: $(YACC_SRC)
	$(YACC) -d $<

$(HCL2C): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

$(HCL2V): $(SRCS)
	$(CC) $(CFLAGS) -DVLOG $^ -o $@

run:
	./$(HCL2C) < example.hcl

run_v:
	./$(HCL2V) < example.hcl

clean:
	rm -rf $(HCL2C) $(HCL2V) $(LEX_OUT).c $(YACC_OUT).* *.o
