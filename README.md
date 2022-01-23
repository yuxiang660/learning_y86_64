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

## Flex程序: yas-grammar.lex
### option配置
```lex
%option noinput
%option nounput
```
* noyywrap表示不需要自定义的`yywrap()`，默认返回1，代表只进行一次文件扫描
    * 例子中自定义了`yywrap()`函数，可在最后打印symbol table
* noinput表示不添加`input()`函数，以消除编译警告`warning: ‘input’ defined but not used`
* nounput表示不添加`yyunput()`函数，以消除编译警告`warning: ‘yyunput’ defined but not used`

### 声明
```lex
%{
#include "yas.h"
%}
```
* 此部分代码会被原样搬到生产的C文件的开头部分
* 头文件里主要包括了相关函数和变量的声明

### 定义段
```lex
%option noyywrap
%option noinput
%option nounput

Instr         rrmovq|cmovle|cmovl|cmove|cmovne|cmovge|cmovg|rmmovq|mrmovq|irmovq|addq|subq|andq|xorq|jmp|jle|jl|je|jne|jge|jg|call|ret|pushq|popq|"."byte|"."word|"."long|"."quad|"."pos|"."align|halt|nop|iaddq
Letter        [a-zA-Z]
Digit         [0-9]
Ident         {Letter}({Letter}|{Digit}|_)*
Hex           [0-9a-fA-F]
Blank         [ \t]
Newline       [\n\r]
Return        [\r]
Char          [^\n\r]
Reg           %rax|%rcx|%rdx|%rbx|%rsi|%rdi|%rsp|%rbp|%r8|%r9|%r10|%r11|%r12|%r13|%r14

/* ERR condition is started if no token is matched */
%x ERR
```
* 各种选项
    * `%option noyywrap`
* 正则表达式和状态定义
    * `Instr`匹配了所有Y86的指令名，所有指令可参考：[Y86-64 Reference](doc/Y86-64 Reference.pdf)
    * `Reg`匹配了15个寄存器ID
* 状态定义
    * `ERR`定义了error状态，通过`BEGIN ERR`跳转到此状态
    * `0`是默认初始状态

### 规则段
```lex
^{Char}*{Return}*{Newline}      {save_line(yytext); REJECT;} /* Snarf input line */
#{Char}*{Return}*{Newline}      {finish_line(); lineno++;}
"//"{Char}*{Return}*{Newline}   {finish_line(); lineno++;}
"/*"{Char}*{Return}*{Newline}   {finish_line(); lineno++;}
{Blank}*{Return}*{Newline}      {finish_line(); lineno++;}

{Blank}+                        ;
"$"+                            ;
{Instr}                         add_instr(yytext);
{Reg}                           add_reg(yytext);
[-]?{Digit}+                    add_num(atoll(yytext));
"0"[xX]{Hex}+                   add_num(atollh(yytext));
[():,]                          add_punct(*yytext);
{Ident}                         add_ident(yytext);
{Char}                          {; BEGIN ERR;}
<ERR>{Char}*{Newline}           {fail("Invalid line"); lineno++; BEGIN 0;}
```
* `^{Char}*{Return}*{Newline}`
    * 匹配非注释行
    * 通过`save_line`保存当前行到全局变量：`input_line`
    * `REJECT` directs the scanner to proceed on to the "second best" rule which matched the input (or a prefix of the input)
        * 此处会对当前行继续匹配下面的规则

* `{Blank}*{Return}*{Newline}`
    * 匹配换行符，表示一行结束
    * 通过`finish_line`处理当前行的解析出的token，进入`finish_line`的时候，当前行的token都已经解析完成，并且存入了`tokens`数据中:
        * token的类型有：`{ TOK_IDENT, TOK_NUM, TOK_REG, TOK_INSTR, TOK_PUNCT, TOK_ERR }`
        * token的内容是：
        ```cpp
        /* Token representation */
        typedef struct {
            char *sval; /* String    */
            word_t ival;   /* Integer   */
            char cval;  /* Character */
            token_t type; /* Type    */
        } token_rec;
        ```
    * 处理label
        * "TOK_IDENT+TOK_PUNCT"，例如：`stack:`
        * 通过`add_symbol`将TOK_IDENT和当前位置`bytepos`存入`symbol_table`
        * 如果当前行只有label，通过`start_line()`重置后，结束当前行的处理，否则继续
    * 处理instruction
        * "TOK_INSTR"，除了label，非注释行都应该以instruction开头
        * `.pos`和`.align`两个instruction需要特殊处理
            * 如果是`.pos`，通过`bytepos = tokens[tpos].ival`调整bytepos
            * 如果是`.align`，通过`bytepos = ((bytepos + a - 1) / a) * a`调整对齐位置，a是align的大小
        * 普通instruction的结构:
            * `find_instr`从`instruction_set`找到instruction的结构后，更新bytepos
            ```cpp
            typedef struct
            {
                const char *name;
                unsigned char code; /* Byte code for instruction+op */
                int bytes;
                arg_t arg1;
                int arg1pos;
                int arg1hi; /* 0/1 for register argument, # bytes for allocation */
                arg_t arg2;
                int arg2pos;
                int arg2hi; /* 0/1 */
            } instr_t;
            ```

* `{Ident}`
    * 匹配标识符
    * 通过`add_token(TOK_IDENT, s, 0, ' ')`添加token

* `[-]?{Digit}+`和`"0"[xX]{Hex}+`
    * 匹配数字，支持十进制和十六进制
    * 通过`add_token(TOK_NUM, NULL, i, ' ')`添加token

* `{Reg}`
    * 匹配寄存器ID
    * 通过`add_token(TOK_REG, s, 0, ' ')`添加token

* `{Instr}`
    * 匹配Y86的指令名
    * 通过`add_token(TOK_INSTR, s, 0, ' ')`添加token

* `[():,]`
    * 匹配单字符：`(`, `)`或者`,`
    * 通过`add_token(TOK_PUNCT, NULL, 0, c)`添加token

### C函数定义
* [yas-grammar.lex](assembler/yas-grammar.lex)没有此段内容，相关C函数和main函数都定义在了其他源文件内

## 主程序main
main函数定义在[yas.c](assembler/yas.c)中，它包括了两次pass
* pass1
    * 检查汇编代码是否有语法错误
    * `add_symbol`
* pass2
    * 在pass1的基础上，处理instruction
    * 通过`print_code`，打印解析结果

### 处理instruction
![inst_layout](pictures/inst_layout.png)

* instruction的结构由`instruction_set`定义：
    * 以`{"irmovq", HPACK(I_IRMOVQ, F_NONE), 10, I_ARG, 2, 8, R_ARG, 1, 0}`为例：
        * 长度为10
        * 第一个argument是I_ARG
        * 第二个argument是R_ARG

如果pass==2，`finish_line`会在最后处理instruction，通过下面的函数，向`char code[10]`填入对应的值，得到当前instruction的机器码：
* `get_reg`处理R_ARG
    * 根据`arg_pos`和`arg_hi`，向`code`中的对应位置填入寄存器ID
* `get_mem`处理M_ARG
    * 根据memory参数的位置，向`code`中的对应位置填入寄存器ID和偏移地址
    * 包含M_ARG的指令只有两类：rmmovq和mrmovq，其中M_ARG的格式可能是(没有Reg的直接使用Num/Ident当作memory的地址)：
        * Num(Reg), (Reg), Num, Ident, Ident(Reg)
* `get_num`处理I_ARG
    * 根据立即数在code中的位置和立即数的长度，向`code`中的对应位置填入立即数


