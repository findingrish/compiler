// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in

#ifndef COMPILER_SYMBOL_H
#define COMPILER_SYMBOL_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//typedef struct ast_node Ast;
typedef struct declnode
{
	int name;
	int dataType;
	int width;
	struct declnode *next;	
	int offset;
	int used;
	int iter;
	int line;
}declarationNode;

typedef struct functionnode
{

	declarationNode *inputParam;
	declarationNode *outputParam;
	declarationNode *declaration;
	int name;
	struct functionnode *next;

}functionNode;

typedef struct recordnode
{	

	int name;
	int width;
	declarationNode *fields;
	struct recordnode *next;

}recordNode;

typedef struct trie{

  int isleaf;
  int id;
  struct trie *child[123];

}trie;

typedef struct symbolTable{

	recordNode *records;
	functionNode *functions;
	declarationNode *globals;

}symbolTable;

typedef struct float1{
	char value[20];
	char name[20];
}Float;

Float floatarr[100];
int floatarrindex;

trie *variableTrie;
int globalNum;
int intwidth;
int realwidth;

char **variableindex;
int variableindexsize;
void* getNode(int a);

int getNumAfter(trie *root,char *st);
symbolTable* populateSymbolTable(Ast *root,int flag);

#endif //COMPILER_SYMBOL_H