// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in


#ifndef COMPILER_AST_H
#define COMPILER_AST_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//typedef struct ast_node Ast;

struct ast_node
{
  Symbol data;
  struct ast_node* parent;
  int numChild;
  TOKEN_INFO* token;
  struct ast_node* child;
  struct ast_node* next;
  struct ast_node* prev;
  int type;
  int isfloat;
  int index;
  int labelindex;
};

typedef struct ast_node Ast;

extern Ast* getAstNode();
extern void copy(Ast *x,ParseTreeNode *y);
extern ParseTreeNode* getptr(treeListNode *head,int pos);
extern void addchild(Ast *x,Ast *y);
extern void add(Ast *x,Ast *y);
extern Ast* convert(ParseTreeNode *root);
extern int printAst(Ast *x,char *s,int flag);
#endif //COMPILER_AST_H