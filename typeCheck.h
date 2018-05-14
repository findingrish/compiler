// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in


#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int assigniter;
// //typedef struct ast_node Ast;
// int findSymbol(char *function,char *symbol,char *field,symbolTable *st,int setused,int iter,int);
void printfloat(FILE *fp);
void checkandExtract(Ast *root,symbolTable *st);
int findSymbol(char *function,char *symbol,char *field,symbolTable *st,int setused,int iter1,int iter2,int checkiter);
#endif //COMPILER_TYPE_H