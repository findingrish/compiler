// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in

#ifndef COMPILER_PARSERDEF_H
#define COMPILER_PARSERDEF_H


#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define NUM_RULES  90
#define MAX_LEN 25

typedef struct trienode{
  int num;
  int isleaf;
  int id;
  struct trienode *child[90];
}trieNode;
////////////////////////////////////////  Structure of Symbol /////////////////////////////////////
typedef struct symbol
{
  char name[MAX_LEN];
  int id;
  int isNonTerminal;
}Symbol;

////////////////////////////////// Integer LinkedList Node ///////////////////////////////////////
typedef struct intnode{
  struct intnode *prev,*next;
  int data;

}intNode;

///////////////////////////////////// Integer LinkedList ////////////////////////////////////////
typedef struct intlinklist{
  intNode *head,*tail;
}intLinkedList;


///////////////////////////////////// Node of LinkedList of Symbol ////////////////////////////////
typedef struct node{
  struct node *prev,*next;
  Symbol data;
}Node;

////////////////////////////////////// LinkedList of Symbol ///////////////////////////////////////
typedef struct linklist{

  Node *head,*tail;

}LinkedList;

/////////////////////////////////////// Structure of Rule //////////////////////////////////////////
typedef struct
{
  int ruleNo;
  Symbol lhs;
  LinkedList* rhs;
  int lenRhs;

}Rule;

//////////////////////////////////// Structure for grammar /////////////////////////////////////////
typedef struct
{
  Rule rule[NUM_RULES];
}Grammar;

typedef struct grammarpair
{
  Grammar reducedGrammar;
  Grammar totalGrammar;
}Gpair;

/////////////////////////////////// Node for first Set /////////////////////////////////////////////
typedef struct
{
  int *arr;
  int isNullable;
  int start; // Starting Rule Number
  int end;  // Ending rule Number
  char name[MAX_LEN];

}FirstSetNode;
//

////////////////////////////////// Node for Follow Set //////////////////////////////////////////////
typedef struct
{
  int *arr;
  char name[MAX_LEN];
  intLinkedList *occur;

}FollowSetNode;

typedef struct treeListNode treeListNode;
typedef struct parsetree_node ParseTreeNode;

typedef struct stack_node
{
  Symbol data;
  struct parsetree_node* ParseTreeNode;
  struct stack_node *next,*prev;
}Stack_node;

typedef struct stack
{
  Stack_node *head,*tail;
}Stack;


struct treeListNode{
  struct treeListNode *prev,*next;
  ParseTreeNode* data;
};

////////////////////////////////////// LinkedList of Symbol ///////////////////////////////////////
typedef struct treelinklist{
  treeListNode *head,*tail;

}TreeLinkedList;

struct parsetree_node
{
  Symbol data;
  struct parsetree_node* parent;
  int numChild;
  TOKEN_INFO* token;
  TreeLinkedList* child;
  int ruleno;
};

typedef struct synch{
    int *arr;
    char name[MAX_LEN];
    intLinkedList *occur;
}Synch;


char **hashTerminal,**hashNonTerminal;
int lenTerminal,lenNonTerminal;
int NonNullableRules;

trieNode *terminalTrie;
trieNode *nonterminalTrie;
FirstSetNode *Firstset;
FollowSetNode *FollowSet;
Synch *synch;

#endif //COMPILER_PARSERDEF_H