// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in

#include "lexerDef.h"

int NUM_KEYWORD = 0;

void insertInTrie1(trieNode1 *root,char *st,int id)
{
  int j ;
  int len = strlen(st),i;
  trieNode1 *head = root;
  trieNode1 *x;
  for(i = 0;i < len;i++)
  {
    if(head->child[st[i]-'$'] == NULL)
    {
      x = (trieNode1 *)malloc(sizeof(trieNode1));
      for(j = 0;j < 26;j++)
      {
        x->child[j] = NULL;
      }   
      x->num = st[i]-'$'; 
      head->child[st[i]-'$'] = x;
      head = head->child[st[i]-'$'];   
    }
    else
    {
      head = head->child[st[i]-'$'];
    }
  }
  head->isleaf = 1;
  head->id = id;

}

int findInTrie1(trieNode1 *root,char *st)
{
  if(root == NULL)
    return -1;
  int len = strlen(st),i;
  for(i = 0;i < len;i++)
  {
    if(root->child[st[i]-'$'] == NULL)
      return -1;
    root = root->child[st[i]-'$'];
  }
  if(root->isleaf)
  return root->id;
  else
    return -1; 
}

// Returns a new DFA
DFA *getNewDFA()
{
  DFA *dfa = NULL;
  dfa = malloc(sizeof(DFA));

  if (dfa==NULL)
  {
    fprintf(stderr, "Failed to allocate memory for DFA\n");
    return dfa;
  }

  // Initially the DFA has no states and current state is undefined
  dfa->start_state = 0;
  dfa->total_states = NULL;
  dfa->cur_state = 0;

  return dfa;
}

// Checks if a state is final or not.
int final_or_not(DFA *dfa, int cur_state)
{
  return dfa->total_states[cur_state].is_final;
}

// Checks whether inp is extendable with cur_state.
int extendable_or_not(DFA *dfa, int cur_state, char inp)
{
  return (dfa->total_states[cur_state].next_state[(int) inp])!=NULL;
}

// Check if a string st is keyword from hash_keyword_token.
int check_hash(char *st)
{
  int i;

  for (i = 0; i < NUM_KEYWORD; i++)
  {
    if (!strcmp(st, hash_keyword_token[i]))
      return 1;
  }

  return 0;
}

// Set the number of states for the dfa and initialize the next_state pointer of every state to NULL.
void setNumStates(DFA *dfa, int numStates)
{

  if (dfa==NULL)
  {
    fprintf(stderr, "Error Initialize dfa");
    exit(-1);
  }
  int i, j;

  dfa->num_states = numStates;
  dfa->total_states = (STATE *) malloc(sizeof(STATE)*numStates);

  for (i = 0; i < numStates; i++)
  {
    dfa->total_states[i].state_number = i;
    dfa->total_states[i].is_final = 0;
    for (j = 0; j < LIMIT; j++)
    {
      dfa->total_states[i].next_state[j] = NULL;
    }
  }
}

/* Inserts transition into DFA.
 * If inptype == 0 i.e single character than input is ascii value of actual input.
 * If inptype == 1 i.e character range than input is index of that range in mappings array.
 */
void transition(int input, int start, int end, int inptype, DFA *dfa, CHAR_RANGE *mappings)
{
  if (inptype==0)
  {
    dfa->total_states[start].next_state[input] = &(dfa->total_states[end]);
  }
  else
  {
    int i;
    for (i = mappings[input - 48].start; i <= mappings[input - 48].end; i++)
    {
      dfa->total_states[start].next_state[i] = &(dfa->total_states[end]);
    }
  }

}

// Return state number of next state after consuming inp on curr_st.
int get_transition(DFA *dfa, int curr_st, char inp)
{
  STATE *state_ptr = dfa->total_states[curr_st].next_state[(int) inp];
  if (state_ptr==NULL)
    return 0;
  return state_ptr->state_number;
}

// Populate the DFA structure by reading from filename containing DFA information in prescribed format.
DFA *populateDfa(char *filename)
{
  DFA *dfa;
  dfa = getNewDFA();

  FILE *fp;
  fp = fopen(filename, "r");

  if (fp==NULL)
  {
    fprintf(stderr, "%s file not found", filename);
    exit(-1);
  }

  char buffer[1000];
  int numStates, numFinalStates, numMapping, i, m;

  fscanf(fp, "%s %d", buffer, &numStates);
  setNumStates(dfa, numStates);

  fscanf(fp, "%s %d", buffer, &numFinalStates);

  //To skip lines
  fgets(buffer, sizeof(buffer), fp);
  fgets(buffer, sizeof(buffer), fp);
  fgets(buffer, sizeof(buffer), fp);

  hashFinalToken = (char **) malloc(sizeof(char *)*numStates);

  for (i = 0; i < numFinalStates; i++)
  {
    bzero(buffer, sizeof(buffer));
    fscanf(fp, "%d %s", &m, buffer);

    hashFinalToken[m] = (char *) malloc(sizeof(char)*TOKEN_LEN);
    strcpy(hashFinalToken[m], buffer);

    dfa->total_states[m].is_final = 1;
  }

  fscanf(fp, "%s %d", buffer, &numMapping);
  CHAR_RANGE mappings[numMapping];

  fgets(buffer, sizeof(buffer), fp);
  fgets(buffer, sizeof(buffer), fp);
  fgets(buffer, sizeof(buffer), fp);

  for (i = 0; i < numMapping; i++)
  {
    bzero(buffer, sizeof(buffer));
    fscanf(fp, "%d %d %d %s", &mappings[i].start, &mappings[i].end, &mappings[i].id, buffer);
  }

  fscanf(fp, "%s %d", buffer, &NUM_KEYWORD);
  hash_keyword_token = (char **) malloc(sizeof(char *)*NUM_KEYWORD);

  for (i = 0; i < NUM_KEYWORD; i++)
  {
    hash_keyword_token[i] = (char *) malloc(sizeof(char)*30);
    fscanf(fp, "%s", hash_keyword_token[i]);
  }

  fgets(buffer, sizeof(buffer), fp);
  fgets(buffer, sizeof(buffer), fp);
  fgets(buffer, sizeof(buffer), fp);

  int curr_st, next, inp_type, x;
  char inp[2];

  while (fscanf(fp, "%d %d %s %d", &curr_st, &inp_type, inp, &next)!=-1)
  {
    // no transition from curr_st
    if (inp_type==2)
    {
      bzero(buffer, sizeof(buffer));
      continue;
    }

    // checking for 10 i.e. /n
    if (strlen(inp) > 1)
    {
      x = atoi(inp);
    }
    else
    {
      // getting ascii value
      x = (int) inp[0];
    }

    bzero(inp, 2);
    transition(x, curr_st, next, inp_type, dfa, mappings);
  }
  return dfa;
}

//Returns buff after reading blocksize from file pointed by fp.
int getNextStream(FILE *fp, char *buff, int blocksize)
{
  return (int) fread(buff, sizeof(char), (size_t) blocksize - 1, fp);
}

//Returns token id of the state identified by state_num.
char *get_token_id(char *st, int state_num)
{
  char *token_id;

  //check if keyword
  if (check_hash(st)==1)
  {
    if (!strcmp(st, "_main"))
    {
      token_id = (char *) malloc(sizeof(char)*(strlen(st) + 2));
      strcpy(token_id, "TK_MAIN");
    }
    else
    {
      token_id = (char *) malloc(sizeof(char)*(strlen(st) + 3));
      int i;
      for (i = 0; i < strlen(st); i++)
        st[i] = (char) toupper(st[i]);
      sprintf(token_id, "TK_%s", st);
    }
  }
  else
  {
    token_id = (char *) malloc(sizeof(char)*(strlen(hashFinalToken[state_num])));
    strcpy(token_id, hashFinalToken[state_num]);
  }
  return token_id;
}

//Checks if c is delimiter or not
int check_delimiter(char c)
{
  if (c=='\n' || c==' ' || c=='\t' || c=='\r')
    return 1;
  return 0;
}

//Creates string usign buff from start to end position.
char *make_string(char *buff, int start, int end)
{
  char *st = (char *) malloc(sizeof(char)*(end - start + 2));
  bzero(st, sizeof(st));
  int i;
  for (i = start; i <= end; i++)
    st[i - start] = buff[i];
  st[i - start] = '\0';
  return st;
}

//Returns TOKEN_INFO structure after populating it with information
TOKEN_INFO *create_token(char *st, int prev_state, int line_num)
{
  TOKEN_INFO *token = (TOKEN_INFO *) malloc(sizeof(TOKEN_INFO));

  token->line_number = line_num;

  token->token_name = (char *) malloc(sizeof(char)*strlen(st));
  strcpy(token->token_name, st);

  char *token_id = get_token_id(st, prev_state);
  token->token_id = (char *) malloc(sizeof(char)*strlen(token_id));
  strcpy(token->token_id, token_id);

  return token;
}

//Returns tokens one after one.
TOKEN_INFO *getNextToken(FILE *fp, FILE *fp_error, FILE *fp_comment, int reset)
{
  static int blocksize ;
  // int blocksize = 3;

  static char *buff[2];
  static int exit_flag = 0, first_run = 0, first_initialization = 0;
  static DFA *dfa;

  int curr_length = 0;

  static int stream_len = 0;
  static int line_num = 1, continue_flag = 0;
  static int start = 0, forward = 0, prev_final = 0, curr_extendable;
  static char *prev_string = NULL;

  static int buff_curr_ind;

  char *st;

  if(reset)
  {
    if(first_initialization)
    {
      bzero(buff[0], sizeof(buff[0]));
      bzero(buff[1], sizeof(buff[1]));
    }

    // fseek(fp, 0, SEEK_SET);
    // fseek(fp_error, 0, SEEK_SET);
    // fseek(fp_comment, 0, SEEK_SET);

    exit_flag = 0;
    first_run = 0;
    curr_length = 0;
    line_num = 1;
    continue_flag = 0;
    start = 0;
    forward = 0;
    prev_final = 0;
    curr_extendable = 0;
    prev_string = NULL;
    buff_curr_ind = 0;

    return NULL;
  }

  if(!first_initialization)
  {
    first_initialization = 1;

    //Getting system blocksize.
    struct stat fi;
    stat("/", &fi);
    blocksize = (int) fi.st_blksize;

    buff[0] = (char *) malloc(sizeof(char)*blocksize);
    buff[1] = (char *) malloc(sizeof(char)*blocksize);

    dfa = populateDfa("dfa.txt");
  }

  //When this function is called for first time.
  if (!first_run)
  {
    first_run = 1;
    stream_len = getNextStream(fp, buff[0], blocksize);

    buff_curr_ind = 0;
    buff[buff_curr_ind][blocksize - 1] = '\0';
  }

  //Whenever this function is called dfa would start from start state i.e 0
  dfa->cur_state = 0;

  if (exit_flag==1)
  {
    TOKEN_INFO *t=(TOKEN_INFO *)malloc(sizeof(TOKEN_INFO));
    t->token_id=(char*)malloc(sizeof("$"));
    strcpy(t->token_id,"$");
    t->token_name=(char*)malloc(sizeof("$"));
    strcpy(t->token_name,"$");
    t->line_number = line_num;
    return t;
  }

  while (stream_len!=0)
  {
    // printf("%s\n", buff[0]);
    for (; forward < blocksize - 1; forward++)
    {
      curr_extendable = extendable_or_not(dfa, dfa->cur_state, buff[buff_curr_ind][forward]);

      if (curr_extendable)
      {
        curr_length++;
        dfa->cur_state = get_transition(dfa, dfa->cur_state, buff[buff_curr_ind][forward]);

        // save current consumed input(s) in prev_String which would be used by inputs coming in next buffer read.
        if (forward==blocksize - 2)
        {
          // if already prev_string is there
          if (prev_string!=NULL && strlen(prev_string))
          {
            char *s = prev_string;
            prev_string = (char *) malloc(sizeof(char)*(strlen(prev_string) + (forward - start + 2)));
            sprintf(prev_string, "%s%s", s, make_string(buff[buff_curr_ind], start, forward));
          }
          else
            prev_string = make_string(buff[buff_curr_ind], start, forward);

          continue_flag = 1;
        }

        if (buff[buff_curr_ind][forward]=='\n')
          line_num++;

        continue;
      }

      //Checking when dfa is at non start state i.e some input(s) have been consumed.
      if (dfa->cur_state!=0)
      {
        prev_final = final_or_not(dfa, dfa->cur_state);

        //if last input in consumed input(s) is at final.
        if (prev_final)
        {
          // if inter buffer inputs are present.
          if (continue_flag)
          {
            st = (char *) malloc(sizeof(char)*(strlen(prev_string) + forward - start + 1));
            sprintf(st, "%s%s", prev_string, make_string(buff[buff_curr_ind], start, forward - 1));
            bzero(prev_string, sizeof(prev_string));
            continue_flag = 0;
          }
          else
          {
            st = make_string(buff[buff_curr_ind], start, forward - 1);
          }

          //checking for /n because it would be a comment
          if (dfa->cur_state!=17)
          {
            int rm = dfa->cur_state;

            //checking if input is of type TK_FUNID and length > 30
            if ((rm==13 || rm==14 || rm==15) && curr_length > 30)
            {
            	if(print_syntax_error)
		      fprintf(stdout,
		              "ERROR_1: FunctionId at line<%d>:<%s> is longer than prescribed length of 30 characters\n",
		              line_num,
		              st);
              error1 = 1;
            }
            else if ((rm==5 || rm==6 || rm==7) && curr_length > 20) //checking if input is of type TK_ID and length > 20
            {
            if(print_syntax_error)
              fprintf(stdout,
                      "ERROR_1: Identifier at line<%d>:<%s> is longer than prescribed length of 20 characters\n",
                      line_num,
                      st);
              error1 = 1;
            }
            else
            {
              start = forward;
              return create_token(st, dfa->cur_state, line_num);
            }
          }
          else
          {
            //fprintf(fp_comment, "%s\n", st);
          }

          // free(st);
          // free(prev_string);
          curr_length = 0;
        }
        else
        {
          //Since last consumed input is at non final state this means it is error.
          if (continue_flag)
          {
            st = (char *) malloc(sizeof(char)*(strlen(prev_string) + forward - start + 1));
            sprintf(st, "%s%s", prev_string, make_string(buff[buff_curr_ind], start, forward - 1));
            bzero(prev_string, sizeof(prev_string));
            continue_flag = 0;
          }
          else
          {
            st = make_string(buff[buff_curr_ind], start, forward - 1);
          }
          if(print_syntax_error)
          fprintf(stdout, "ERROR_3: Unknown pattern <%s> at line <%d>\n", st, line_num);
          error1 = 1;
        }

        dfa->cur_state = 0;
        start = forward;

        //Condition when at boundary of buffer and current input is valid input.
        if (forward==blocksize - 2 && (!check_delimiter(buff[buff_curr_ind][forward]) && !buff[buff_curr_ind][forward]=='\0'))
        {
          if (extendable_or_not(dfa, dfa->cur_state, buff[buff_curr_ind][forward]))
          {
            dfa->cur_state = get_transition(dfa, dfa->cur_state, buff[buff_curr_ind][forward]);
            prev_string = make_string(buff[buff_curr_ind], start, forward);
            continue_flag = 1;
          }
          forward += 1;
        }

        forward -= 1;
      }
      else  // when dfa is at start state i.e previous consumed input was delimiter or invalid input.
      {
        if (check_delimiter(buff[buff_curr_ind][forward]))
        {
          if (buff[buff_curr_ind][forward]=='\n')
            line_num++;
          start = forward + 1;
          continue;
        }

        //when EOF has been reached.
        if (stream_len < blocksize && forward >= stream_len)
        {
          exit_flag = 1;
          if (final_or_not(dfa, dfa->cur_state))
          {
            bzero(buff[buff_curr_ind], sizeof(buff[buff_curr_ind]));
            return create_token(make_string(buff[buff_curr_ind], start, forward - 1), dfa->cur_state, line_num);
          }
          break;
        }
	
	if(print_syntax_error)
		fprintf(stdout, "ERROR_2: Unknown symbol <%s> at line <%d>\n", make_string(buff[buff_curr_ind], start, forward),
		        line_num);
	error1 = 1;
        start = forward + 1;
        continue;
      }
    }

    buff_curr_ind = !buff_curr_ind;

    //Reload buffer again.
    bzero(buff[buff_curr_ind], sizeof(buff[buff_curr_ind]));
    stream_len = getNextStream(fp, buff[buff_curr_ind], blocksize);
    buff[buff_curr_ind][blocksize - 1] = '\0';

    start = 0;
    forward = 0;

    if (exit_flag==1)
      break;
  }

  // if prev_string is present this means it hasnt been saved yet.
  if (prev_string!=NULL && strlen(prev_string) > 0 && final_or_not(dfa, dfa->cur_state))
  {
    exit_flag = 1;
    char *x;
    x = (char *) malloc(sizeof(char)*strlen(prev_string));
    bzero(prev_string, sizeof(prev_string));

    return create_token(x, dfa->cur_state, line_num);
  }

  TOKEN_INFO *t=(TOKEN_INFO *)malloc(sizeof(TOKEN_INFO));
  t->token_id=(char*)malloc(sizeof("$"));
  strcpy(t->token_id,"$");
  t->token_name=(char*)malloc(sizeof("$"));
  strcpy(t->token_name,"$");
  t->line_number = line_num;
  return t;
}
