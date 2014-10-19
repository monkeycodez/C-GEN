/*
 * =============================================================================
 *
 *       Filename:  cgen.h
 *
 *    Description:  Header for cgen
 *
 *        Version:  1.0
 *        Created:  05/10/14 10:06:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Gruda (mg), matthewgruda@gmail.com
 *   Organization:  
 *
 * =============================================================================
 */

#ifndef __C_GEN_H
#define __C_GEN_H 	0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define BEGIN_EVAL 1
#define END_EVAL   2
#define NO_ACTION  0

typedef int (*lang_f)(char *line);

extern FILE* f_out;

extern FILE* f_in;


char *rline(FILE* file);

void eval(lang_f cb, int prtbrk);

#endif 
