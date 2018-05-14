// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in

#ifndef LEXER_H
#define LEXER_H

#include "lexerDef.h"

extern DFA *getNewDFA();
extern int final_or_not(DFA *dfa, int cur_state);
extern int extendable_or_not(DFA *dfa, int cur_state, char inp);
extern int check_hash(char *st);
extern void setNumStates(DFA *dfa, int numStates);
extern void transition(int input, int start, int end, int inptype, DFA *dfa, CHAR_RANGE *mappings);
extern int get_transition(DFA *dfa, int curr_st, char inp);
extern DFA *populateDfa(char *filename);
extern int getNextStream(FILE *fp, char *buff, int blocksize);

extern char *get_token_id(char *st, int state_num);
extern int check_delimiter(char c);
extern char *make_string(char *buff, int start, int end);
extern TOKEN_INFO *getNextToken(FILE *fp, FILE *fp_error, FILE *fp_comment,int reset);
#endif /* LEXER_H */
