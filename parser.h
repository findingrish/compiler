// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "parserDef.h"

void populateHash();

int checkInHash(char *st,int hash);

ParseTreeNode* rootNode;

Gpair populateRules();
int** getParseTable();
void makeparsetable(int **parsetable, FirstSetNode* firstset, FollowSetNode* followset, Grammar g);
void stack_simulate(FILE *fp,FILE *fp_error,FILE *fp_comment, int** parsetable, Grammar g);
void print2(Grammar g,FILE *fp,int len);
void FindFollowSet(Grammar g);
void FindFirstSet(Grammar g);

//void printParseTree(ParseTreeNode* root,FILE *fp_tree);
int printDFSTree(ParseTreeNode* root,int flag);
void setSynch(Grammar g);
void printFirstSet();
void printFollowSet();
// void synch_parsetable(int **parsetable,FirstSetNode* firstset,FollowSetNode* followset);
#endif //COMPILER_PARSER_H