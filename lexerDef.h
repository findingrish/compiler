// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in

#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#define LIMIT 128
#define TOKEN_LEN 20

int error1;
int error2;

int print_syntax_error;
int print_semantic_error;

typedef struct trienode1{
  int num;
  int isleaf;
  int id;
  struct trienode1 *child[90];
}trieNode1;

typedef struct state_struct
{
  int state_number;
  int is_final;
  struct state_struct *next_state[LIMIT];
} STATE;

typedef struct charRange
{
  int start;
  int end;
  int id;
} CHAR_RANGE;

typedef struct dfa_struct
{
  int num_states;
  int start_state;
  int cur_state;
  STATE *total_states;
} DFA;

typedef struct token_struct
{
  int line_number;
  char *token_id;
  char *token_name;
} TOKEN_INFO;

char **hash_keyword_token;

extern int NUM_KEYWORD;

char **hashFinalToken;

trieNode1 *hashTokens;

#endif /* LEXERDEF_H */