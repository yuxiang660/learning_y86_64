#pragma once

/**
 * @file yas.h
 * @author Little Bee (littlebee1024@outlook.com)
 * @brief Funtions used in lex file
 * @date 2021-11-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdint.h>

int yylex();

/**
 * @brief save line
 * 
 */
void save_line(char*);

/**
 * @brief finish line
 * 
 */
void finish_line();

/**
 * @brief add instruction
 * 
 */
void add_instr(char*);

/**
 * @brief add register
 * 
 */
void add_reg(char*);

/**
 * @brief add number
 * 
 */
void add_num(long long);

/**
 * @brief add punct
 * 
 */
void add_punct(char);

/**
 * @brief add id
 * 
 */
void add_ident(char*);

/**
 * @brief fail func
 * 
 */
void fail(const char*);

/**
 * @brief convert hex string to interger
 * 
 * @return unsigned long long 
 */
unsigned long long atollh(const char*);

// Current line number
extern int lineno;
