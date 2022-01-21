# Introduction
This project is used to learning Y86-64 processor simulator. For details, please refer to
* http://csapp.cs.cmu.edu
* https://w3.cs.jmu.edu/lam2mo/cs261_2019_08/y86-intro.html
* https://github.com/sysprog21/y86_64-tools

# Y86-64 Assembler
[Y86-64汇编器](assembler)在`flex`的帮助下将汇编代码转换成了机器码。

## Purpose
To implement a lexer for Y86-64 processor. Then we can use is to convert Y86-64 assembly code to machine code. For example, the assembly code below (right part) can be converted to the machine code below (left part).

```bash
                            | # Execution begins at address 0 
0x000:                      | 	.pos 0
0x000: 30f40002000000000000 | 	irmovq stack, %rsp  	# Set up stack pointer
0x00a: 803800000000000000   | 	call main		# Execute main program
0x013: 00                   | 	halt			# Terminate program 
                            | 
                            | # Array of 4 elements
0x018:                      | 	.align 8
0x018: 0d000d000d000000     | array:	.quad 0x000d000d000d
0x020: c000c000c0000000     | 	.quad 0x00c000c000c0
0x028: 000b000b000b0000     | 	.quad 0x0b000b000b00
0x030: 00a000a000a00000     | 	.quad 0xa000a000a000
                            | 
0x038: 30f71800000000000000 | main:	irmovq array,%rdi
0x042: 30f60400000000000000 | 	irmovq $4,%rsi
0x04c: 805600000000000000   | 	call sum		# sum(array, 4)
0x055: 90                   | 	ret
                            | 
                            | # long sum(long *start, long count)
                            | # start in %rdi, count in %rsi
0x056: 30f80800000000000000 | sum:	irmovq $8,%r8        # Constant 8
0x060: 30f90100000000000000 | 	irmovq $1,%r9	     # Constant 1
0x06a: 6300                 | 	xorq %rax,%rax	     # sum = 0
0x06c: 6266                 | 	andq %rsi,%rsi	     # Set CC
0x06e: 708700000000000000   | 	jmp     test         # Goto test
0x077: 50a70000000000000000 | loop:	mrmovq (%rdi),%r10   # Get *start
0x081: 60a0                 | 	addq %r10,%rax       # Add to sum
0x083: 6087                 | 	addq %r8,%rdi        # start++
0x085: 6196                 | 	subq %r9,%rsi        # count--.  Set CC
0x087: 747700000000000000   | test:	jne    loop          # Stop when 0
0x090: 90                   | 	ret                  # Return
                            | 
                            | # Stack starts here and grows to lower addresses
0x200:                      | 	.pos 0x200
0x200:                      | stack:
```

## Flex程序


