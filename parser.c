// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in

#include "lexer.h"
#include "parser.h"


void insertInTrie(trieNode *root,char *st,int id)
{
	int j ;
	int len = strlen(st),i;
	trieNode *head;
  head = root;
	trieNode *x;
	for(i = 0;i < len;i++)
	{  
		if(head->child[st[i]-'$'] == NULL)
		{
		 	x = (trieNode *)malloc(sizeof(trieNode));
      if(x == NULL)
      {
        printf("eaawasdadsasd\n");
      }
			for(j = 0;j < 90;j++)
			{
				x->child[j] = NULL;
			}		
			x->num = st[i]-'$'; 
			head->child[st[i]-'$'] = x;
			head = x;	 
      if(head == NULL)
      {
        printf("aaasdasdadsadsadsklaklasdadsjasdjasd\n");
      }
		}
		else
		{
			head = head->child[st[i]-'$'];
      if(head == NULL)
      {
        printf("aaasdasdadsadsadsklaklasdadsjasdjasd\n");
      }
		}
	}
	head->isleaf = 1;
	head->id = id;

}

int findInTrie(trieNode *root,char *st)
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
////////////////////////////////// hash = 1 imply terminal  //////////////////////////////////////
int checkInHash(char *st,int hash)
{
  int i;
  if(hash == 1) //check in terminal
  {
    return findInTrie(terminalTrie,st);
  }
  else
  {
    return findInTrie(nonterminalTrie,st);  
  }

}

////////////////////////////////// Fill both the Trie ////////////////////////////////////
void populateHash()
{
	FILE  *f1,*f2;
	f1 = fopen("terminal.txt","r");
	terminalTrie = NULL;
	nonterminalTrie = NULL;
	int j;
	terminalTrie = (trieNode *)malloc(sizeof(trieNode));
  if(terminalTrie == NULL)
  {
    printf("asasdasdasdasdasd\n");
  }
	for(j = 0;j < 90;j++)
	{
		terminalTrie->child[j] = NULL;
	}
	terminalTrie->isleaf = 0;
	
	nonterminalTrie = (trieNode *)malloc(sizeof(trieNode));
	for(j = 0;j < 90;j++)
	{
		nonterminalTrie->child[j] = NULL;
	}
	nonterminalTrie->isleaf = 0;

  char line[MAX_LEN];
  int len1,len2;
  fscanf(f1,"%d",&len1);
  //printf("Len1 %d\n",len1);
  hashTerminal = (char **)malloc(sizeof(char *)*len1);
  int i = 0;
  while(fscanf(f1,"%s",line) > 0)
  {

    hashTerminal[i] = (char *)malloc(sizeof(char)*(strlen(line)+1));
    strcpy(hashTerminal[i],line);
    insertInTrie(terminalTrie,line,i);
    bzero(line,sizeof(line));
    i++;
  }
 fclose(f1);

  f2 = fopen("nonterminal.txt","r");
  fscanf(f2,"%d",&len2);
  //printf("Len2 %d\n",len2);
  hashNonTerminal = (char **)malloc(sizeof(char *)*len2);
  i = 0;
  while(fscanf(f2,"%s",line) > 0)
  {

    //printf("%s\n",line);
    hashNonTerminal[i] = (char *)malloc(sizeof(char)*(strlen(line)+1));
    strcpy(hashNonTerminal[i],line);
    insertInTrie(nonterminalTrie,line,i);
    //printf("%s\n",hashNonTerminal[i]);
    bzero(line,sizeof(line));
    i++;
  }
 
 fclose(f2);
  lenTerminal = len1;
  lenNonTerminal = len2;

}



////////////////////////////////////////  Structure of Symbol /////////////////////////////////////


//////////////////////////////// Initialise First Set ///////////////////////////////////////////////
void setFirstSet()
{
  Firstset = (FirstSetNode *)malloc(sizeof(FirstSetNode)*lenNonTerminal);
  int i= 0;
  for(i = 0;i < lenNonTerminal;i++)
  {
    Firstset[i].arr = (int *)malloc(sizeof(int)*lenTerminal);
    int j;
    for(j = 0;j < lenTerminal;j++)
      Firstset[i].arr[j] = 0;
    Firstset[i].isNullable = 0;
    strcpy(Firstset[i].name,hashNonTerminal[i]);
  }

}


////////////////////////////// Get a integer linkedlist /////////////////////////////////////////////
intLinkedList* getIntLinkedList()
{
  intLinkedList *list = NULL;
  list = malloc(sizeof(intLinkedList));

  if(list == NULL)
  {
    fprintf ( stderr, "Failed to allocate memory for linked list\n" );
    return NULL;
  }
  list->head = NULL;
  list->tail = NULL;
  return list;
}

/////////////////////////// Insert a integer in LinkedList //////////////////////////////////////////
void insertIntAtBack(intLinkedList *a,int b)
{
  intNode *x = (intNode *)malloc(sizeof(intNode));
  x->next = NULL;
  x->data = b;
  if(a->head == NULL)
  {
    x->prev = NULL;
    a->head = x;
    a->tail = x;
  }
  else
  {
    a->tail->next = x;
    x->prev = a->tail;
    a->tail = x;
  }
}


/////////////////////////////// Get a symbol linkedlist  ////////////////////////////////////////////
LinkedList* getLinkedList()
{
  LinkedList *list = NULL;
  list = malloc(sizeof(LinkedList));

  if(list == NULL)
  {
    fprintf ( stderr, "Failed to allocate memory for linked list\n" );
    return NULL;
  }
  list->head = NULL;
  list->tail = NULL;
  return list;
}

///////////////////////// Insert a symbol in LinkedList /////////////////////////////////////////////
void insertAtBack(LinkedList *a,Symbol b)
{
  Node *x = (Node *)malloc(sizeof(Node));
  x->next = NULL;
  x->data = b;
  if(a->head == NULL)
  {
    x->prev = NULL;
    a->head = x;
    a->tail = x;
  }
  else
  {
    a->tail->next = x;
    x->prev = a->tail;
    a->tail = x;
  }
}



//////////////////////// Fill the Grammar data Structure by reading from Grammar.txt //////////////////
Gpair populateRules()
{
  populateHash();
  setFirstSet();
  FILE* fp = fopen("grammar.txt","r");

  Grammar grammar,nullGrammar;
  int ruleNo = 0,lenRhs,nullRuleNo = 0,flag = 0;
  int cur_lhs = -1;
  char line[25];
  int i,hash,index;  // hash indicates whether we have to search in hashTerminal or hashNonTerminal

  while(1)
  {
    lenRhs = 0;
    Rule rule;
    //	rule.ruleNo = ruleNo;
    i = 1;

    if(fscanf(fp,"%s",line) > 0)
    {
      hash = 2; // first string is always nonterminal
      line[strlen(line)-1]= '\0';  // To remove '>'

      index = checkInHash(line+1,2);

      rule.lhs.isNonTerminal = 1;
      strcpy(rule.lhs.name,line+1);
      rule.lhs.id = index;
      if(cur_lhs != index)
      {
        if(cur_lhs == -1)
        {
          cur_lhs = index;
          Firstset[index].start = ruleNo;
        }
        else
        {
          Firstset[cur_lhs].end = ruleNo-1;
          cur_lhs = index;
          Firstset[cur_lhs].start = ruleNo;
        }
      }
      LinkedList *rhs = (LinkedList *)getLinkedList();
      while(1)
      {
        fscanf(fp,"%s",line);
        if(!strcmp(line,"===>"))
          continue;
        // Nonterminal Case
        if(line[0] == '<')
        {
          line[strlen(line)-1] = '\0';
          Symbol b;
          b.isNonTerminal = 1;
          strcpy(b.name,line+1);
          index = checkInHash(line+1,2);
          b.id = index;
          insertAtBack(rhs,b);
          lenRhs++;

        }
        else if(line[0] == 'T')   // Terminal Case
        {
          Symbol b;
          b.isNonTerminal = 0;
          strcpy(b.name,line);
          index = checkInHash(line,1);
          b.id = index;
          insertAtBack(rhs,b);
          lenRhs++;
        }
        else if(line[0] == 'e')
        {
          free(rhs);
          rhs = NULL;
          Firstset[rule.lhs.id].isNullable = 1;
          rule.ruleNo = nullRuleNo;
          rule.rhs = rhs;
          nullGrammar.rule[nullRuleNo] = rule;
          nullRuleNo++;
          fscanf(fp,"%s",line);
          break;
        }
        else if(line[0] == '.')
        {
          // Same rule goes both into grammar and nullGrammar with different rule numbers
          rule.ruleNo = ruleNo;
          rule.rhs = rhs;
          rule.lenRhs = lenRhs;
          grammar.rule[ruleNo] = rule;
          rule.ruleNo = nullRuleNo;
          nullGrammar.rule[nullRuleNo] = rule;
          ruleNo++;
          nullRuleNo++;
          break;
        }
      }
    }
    else{
      Firstset[cur_lhs].end = ruleNo-1;
      break;
    }

  }

 fclose(fp);
  NonNullableRules = ruleNo;
  Gpair g;
  g.reducedGrammar = grammar;
  g.totalGrammar = nullGrammar;
  return g;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// Finding First Set    ////////////////////////////////////////

void recurFirstSet(int i,int *visited,Grammar g)
{
  if(!visited[i])
  {
    //printf("%s Started\n",g.rule[Firstset[i].start].lhs.name);
    int k;
    //int flag = 0;

    for(k = Firstset[i].start;k <= Firstset[i].end; k++)
    {
      Rule r = g.rule[k];
      Node *rhs = r.rhs->head;
      Symbol s = rhs->data;
      if(s.isNonTerminal == 0){
        int index = checkInHash(s.name,1);

        Firstset[i].arr[index] = 1;
      }
      else
      {
        while(1)
        {

          if(s.isNonTerminal == 0)
          {
            int index = checkInHash(s.name,1);
            Firstset[i].arr[index] = 1;
            break;
          }
          recurFirstSet(s.id,visited,g);
          //arrayor(Firstset[i].arr,Firstset[s.id].arr,lenNonTerminal);
          int l;
          for(l = 0;l < lenTerminal;l++)
            Firstset[i].arr[l] = Firstset[i].arr[l] | Firstset[s.id].arr[l];
          if(Firstset[s.id].isNullable){
            if(rhs != NULL){
              rhs = rhs->next;
              s = rhs->data;
            }
            else{
              Firstset[i].isNullable = 1;
              break;
            }
          }
          else{

            break;
          }
        }
      }
    }
    //printf("%s Done\n",g.rule[Firstset[i].start].lhs.name);
    visited[i] = 1;
  }

}

void FindFirstSet(Grammar g)
{

  int visited[lenNonTerminal];
  int i;
  for(i = 0;i < lenNonTerminal;i++)
    visited[i] = 0;

  for(i = 0;i < lenNonTerminal;i++)
  {
    if(visited[i])
      continue;
    recurFirstSet(i,visited,g);
  }

}

///////////////////////////////////////  Ending First Set    //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////   Finding Follow Set  ///////////////////////////////////////

// Return the Pointer to the Nonterminal represented by i.
Node* findInRhs(Rule r,int i)
{
  Node *rhs = r.rhs->head;
  Symbol s;
  while(rhs != NULL)
  {
    s = rhs->data;
    if(s.isNonTerminal && i == s.id)
    {
      return rhs;
    }
    rhs = rhs->next;
  }
  return NULL;
}

// Take or of the array.
void arrayor(int *dest,int *src,int len)
{
  if(dest == NULL || src == NULL)
  {
    fprintf(stderr, "%s\n","Provide valid input for Oring" );
    return;
  }
  int i;
  for(i = 0;i < len;i++)
  {
    dest[i] = dest[i] | src[i];
  }
}

// Initialise follow set.Called from FindFollowSet()
void setFollowSet(Grammar g)
{

  int i;

  FollowSet = (FollowSetNode *)malloc(sizeof(FollowSetNode)*lenNonTerminal);

  for(i = 0;i < lenNonTerminal;i++)
  {
    FollowSet[i].arr = (int *)malloc(sizeof(int)*lenTerminal);
    int j;
    for(j = 0;j < lenTerminal;j++)
      FollowSet[i].arr[j] = 0;
    strcpy(FollowSet[i].name,hashNonTerminal[i]);
    FollowSet[i].occur = getIntLinkedList();
  }

  FollowSet[0].arr[56] = 1; // Follow of <program> is $.

  for(i = 0;i < NonNullableRules;i++)
  {
    Rule r = g.rule[i];
    Node *rhs = r.rhs->head;
    Node *next;
    Symbol s;
    // Iterate on any rule and find all nonterminals keep udating there follow sets
    // Ignore cycles now.
    while(rhs != NULL)
    {
      // If a nonterminal is found in the rhs update its follows.
      if(rhs->data.isNonTerminal)
      {
        next = rhs->next; // Pointer to next of a nonterminal.
        s = rhs->data; // s is symbol of the nonterminal being considered.
        while(1)
        {
          if(next == NULL)
          {
            if(s.id == r.lhs.id)
              break;
            insertIntAtBack(FollowSet[s.id].occur,r.lhs.id);
            arrayor(FollowSet[s.id].arr,FollowSet[r.lhs.id].arr,lenTerminal);
            break;
          }
          else
          {
            if(next->data.isNonTerminal == 0)  // If  next is a terminal.
            {
              FollowSet[s.id].arr[next->data.id] = 1;
              break;
            }
            else // If next is not a terminal.
            {
              arrayor(FollowSet[s.id].arr,Firstset[next->data.id].arr,lenTerminal);
              if(Firstset[next->data.id].isNullable)
              {
                next = next->next;
              }
              else
              {
                break;
              }
            }
          }
        }
      }
      rhs = rhs->next;
    }
  }

}

// Dependent on FindFollowSet function
void recurFollowSet(int i,int *visited,Grammar g,int *parent)
{
  //printf("%s Started\n",hashNonTerminal[i]);
  int index;
  parent[i] = 1;
  if(!visited[i])
  {
    // Debug
    if(FollowSet[i].occur->head != NULL)
    {
      intNode *h = FollowSet[i].occur->head;

      while(h != NULL)
      {
        if(parent[h->data] == 0)
          recurFollowSet(h->data,visited,g,parent);
        arrayor(FollowSet[i].arr,FollowSet[h->data].arr,lenNonTerminal);
        h = h->next;
      }
    }
    visited[i] = 1;
    // Debug
  }

  //printf("%s Ended\n",hashNonTerminal[i]);
}

// Call this to calculate FollowSet.
void FindFollowSet(Grammar g)
{
  setFollowSet(g);
  int *visited,*parent;
  int i;
  visited = (int *)malloc(sizeof(int)*lenNonTerminal);
  parent = (int *)malloc(sizeof(int)*lenNonTerminal);
  for(i = 0;i < lenNonTerminal;i++){
    visited[i] = 0;
    parent[i] = 0;
  }

  visited[0] = 1;

  FollowSet[0].arr[56] = 1;  // for $.

  for(i = 1;i < lenNonTerminal;i++)
  {
    if(FollowSet[i].occur == NULL)
    {
      visited[i] = 1;
    }
  }
  int j;

  for(i = 1;i < lenNonTerminal;i++)
  {
    if(visited[i])
      continue;

    recurFollowSet(i,visited,g,parent);
    for(j = 0;j < lenNonTerminal;j++)
      parent[j] = 0;
  }


}
// Print Follow set to FollowSet.txt
void printFollowSet(){

  FILE *fp;
  fp = fopen("FollowSet.txt","w");
  int i,j;
  for(i = 0;i < lenNonTerminal;i++)
  {

    fprintf(fp,"%s -> ",FollowSet[i].name);

    for(j = 0;j < lenTerminal;j++)
    {
      if(FollowSet[i].arr[j] == 0)
        continue;


      fprintf(fp ,"%s ",hashTerminal[j]);

    }

    fprintf(fp,"%s",".\n");
  }
//  fclose(fp);
  fclose(fp);
}


////////////////////////////////////////  Follow ends ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Print first set to FirstSet.txt ///////////////////////
void printFirstSet()
{
  FILE *fp;

  fp = fopen("FirstSet.txt","w");
  int i,j;
  for(i = 0;i < lenNonTerminal;i++)
  {
    fprintf(fp,"%s ",Firstset[i].name);
    for(j = 0;j < lenTerminal;j++)
    {
      if(Firstset[i].arr[j] == 0)
        continue;
      fprintf(fp,"%s ",hashTerminal[j]);
    }
    fprintf(fp, "%s", ".\n");
  }
 fclose(fp);
}



//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Print a rule //////////////////////////////////////
void print(Rule r,int i,FILE *fp)
{
  fprintf(fp,"%d ",r.ruleNo);
  fprintf(fp,"%s -> ",r.lhs.name);

  LinkedList *l = r.rhs;
  Node *a = NULL;

  if(l != NULL)
    a = l->head;

  while(a != NULL){
    fprintf(fp,"%s ",a->data.name);

    a = a->next;
  }
  fprintf(fp,"%s",".\n");

}


//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Print complete Grammar ///////////////////////////////////
void print2(Grammar g,FILE *fp,int len)
{

  int i = 0;
  for(i = 0; i < len;i++)
    print(g.rule[i],i,fp);

}

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Returns a 2d array ///////////////////////////////////
int** getParseTable()
{
  int **parsetable,i,j,k;
  parsetable = (int **)malloc(sizeof(int*)*lenNonTerminal);
  for(i=0;i<lenNonTerminal;i++)
  {
    parsetable[i] = (int *)malloc(sizeof(int)*lenTerminal);
    for(j=0;j<lenTerminal;j++)
      parsetable[i][j] = -1;
  }
  return parsetable;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Calculate first set for each rule ///////////////////////////////////
FirstSetNode* first_set_by_rule(Grammar g,int ruleind, FirstSetNode* firstset)
{
  Rule rule = g.rule[ruleind];
  int *set = (int *)malloc(sizeof(int) * lenTerminal);
  bzero(set,sizeof(set));
//  int lhs_ind = rule.lhs.id;
  int is_null = 0;
  if (rule.rhs!=NULL)
  {
    Node *rhs = rule.rhs->head;
    while(1)
    {
      if(rhs == NULL)
      {
        is_null = 1;
        break;
      }
      if (rhs->data.isNonTerminal)
      {
        arrayor(set,firstset[rhs->data.id].arr, lenTerminal);
        if (firstset[rhs->data.id].isNullable)
          rhs = rhs->next;
        else
          break;
      }
      else
      {
        set[rhs->data.id] = 1;
        break;
      }
    }
  }
  else
    is_null = 1;

  FirstSetNode* f = (FirstSetNode*) malloc(sizeof(FirstSetNode));
  f->arr = (int *)malloc(sizeof(int)*lenTerminal);

  int i;
  for(i=0;i<lenTerminal;i++)
    f->arr[i] = set[i];
//  memcpy(f->arr,set,sizeof(set));
  f->isNullable = is_null;
  return f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Convert a rule into character form ///////////////////////////////////
char* makerule(Rule r)
{
  char *rule=(char*)malloc(sizeof(char)*200);
  char *lhs = hashNonTerminal[r.lhs.id];
  LinkedList* rhs;
  strcpy(rule,lhs);
  rhs = r.rhs;
  if(rhs!= NULL)
  {
    strcat(rule," ->");
    Node* n=rhs->head;
    while(n!=NULL)
    {
      strcat(rule," ");
      strcat(rule,n->data.name);
      n=n->next;
    }
  }
//  strcat(rule,"\n");
  return rule;
}

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Build Parse Table ////////////////////////////////////////
void makeparsetable(int **parsetable, FirstSetNode* firstset, FollowSetNode* followset, Grammar g)
{
  int i,j,k;
  FirstSetNode *set;
  for(i=0;i<NUM_RULES;i++)
  {
    int lhs_ind = g.rule[i].lhs.id;
    set = first_set_by_rule(g,i,firstset);
    for(j=0;j<lenTerminal;j++)
    {
      if(set->arr[j] != 0)
        parsetable[lhs_ind][j] = i;
    }
    if(set->isNullable)
    {
      int *follow = followset[lhs_ind].arr;
      for(j=0;j<lenTerminal;j++)
      {
        if(follow[j] != 0)
          parsetable[lhs_ind][j] = i;
      }
    }
  }
}

// Print Parse Table
void printparsetable(int **parsetable,Grammar g)
{
  int i,j;
  for(i=0;i<lenNonTerminal;i++)
  {
    char *istr = hashNonTerminal[i];
    for(j=0;j<lenTerminal;j++)
    {
      char *jstr = hashTerminal[j];
      if(parsetable[i][j] != -1)
        printf("%s %s = %s %d\n",istr,jstr,makerule(g.rule[parsetable[i][j]]), parsetable[i][j]);
    }
  }
}


void print_terminals(int *arr)
{
  int i;
  for(i=0;i<lenTerminal;i++)
  {
    if(arr[i] != 0)
      printf("%s\n",hashTerminal[i] );
  }
}

void push_stack(Stack* st,Symbol sym,ParseTreeNode* TreeNode)
{
  Stack_node *node =(Stack_node *) malloc(sizeof(Stack_node));
  node->data.id = sym.id;
  node->data.isNonTerminal = sym.isNonTerminal;
  node->ParseTreeNode = TreeNode;
  strcpy(node->data.name,sym.name);

  if(st->tail !=NULL)
    st->tail->next=node;
  node->prev=st->tail;
  node->next=NULL;
  st->tail=node;
//  st->top = st->tail->data;

  if(st->head == NULL)
    st->head=node;

}

Stack_node* pop_stack(Stack* st)
{
  Stack_node *node = st->tail;

  st->tail=st->tail->prev;
  // if(st->tail == NULL)
  // {
  //   printf("stack underflow. Exitting\n");
  //   exit(-1);
  // }
  if(st->tail!=NULL)
  {
    st->tail->next = NULL;
//    st->top=st->tail->data;
  }
  // else
  // {
  //   st->top=st->ta;
  //   st->head=NULL;
  // }
  return node;

}

void print_stack(Stack* st, FILE* fp_stack)
{
  Stack_node *st_node = st->tail;
  while(st_node!=NULL){
    fprintf(fp_stack,"%s %d\n",st_node->data.name,st_node->data.id);
    st_node=st_node->prev;
  }
  fprintf(fp_stack,"\n\n\n");
}

TreeLinkedList* getTreeLinkedList()
{
  TreeLinkedList *list = NULL;
  list = malloc(sizeof(TreeLinkedList));

  if(list == NULL)
  {
    fprintf ( stderr, "Failed to allocate memory for linked list\n" );
    return NULL;
  }
  list->head = NULL;
  list->tail = NULL;
  return list;
}

ParseTreeNode* getTreeNode()
{
  ParseTreeNode* tree = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
  tree->child = getTreeLinkedList();
  tree->numChild = 0;
  tree->parent = NULL;
  bzero(tree->data.name,0);
  return tree;
}

///////////////////////// Insert a symbol in LinkedList /////////////////////////////////////////////
TreeLinkedList* insertChildTree(Symbol b, ParseTreeNode* parent)
{
  TreeLinkedList *a = parent->child;
  treeListNode *treenode = (treeListNode *)malloc(sizeof(treeListNode));
  ParseTreeNode *x = getTreeNode();
  x->data = b;
  x->parent = parent;
  treenode->data = x;
  treenode->next = NULL;

  if(a!=NULL)
  {
    if(a->head == NULL)
    {
      a->head = treenode;
      a->tail = treenode;
    }
    else
    {
//      a->head=treenode;
      a->tail->next = treenode;
      treenode->prev=a->tail;
      a->tail=treenode;

    }
  }


  return a;
}


Stack* initialize_stack()
{
  Stack *st = (Stack *)malloc(sizeof(Stack));
  st->head = NULL;
  st->tail = NULL;
  Symbol s;

  strcpy(s.name,"$");
  s.id=checkInHash("$",1);
  s.isNonTerminal = 0;
  push_stack(st,s,NULL);

  s.id=0;
  s.isNonTerminal=1;
  strcpy(s.name,hashNonTerminal[0]);
  rootNode = getTreeNode();
  rootNode->data = s;
  push_stack(st,s,rootNode);

  return st;
}

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////     Synch               /////////////////////////////////////////////
char** setKeyword()
{
  char **word;
  word = (char **)malloc(sizeof(char *)*12);
  int i;
  for(i = 0;i < 12;i++)
    word[i] = (char *)malloc(sizeof(char)*15);


    strcpy(word[0],"TK_END");
    strcpy(word[1],"TK_WHILE");
    strcpy(word[2],"TK_PARAMETER");
    strcpy(word[3],"TK_LIST");
    strcpy(word[4],"TK_ENDWHILE");
    strcpy(word[5],"TK_THEN");
    strcpy(word[6],"TK_ENDIF");
    strcpy(word[7],"TK_CALL");
    strcpy(word[8],"TK_ENDRECORD");
    strcpy(word[9],"TK_ELSE");
    strcpy(word[10],"TK_SEM");   
    strcpy(word[11],"TK_IF");
  return word;  

}

int checkInSynch(int id,char **word)
{

   int len = 11,i; 
  

  int arr[len];
  memset(arr,0,sizeof(arr));
  for(i = 0;i < len;i++)
  {
    arr[i] = checkInHash(word[i],1);
  }

  for(i = 0;i < len;i++)
  {
    if(id == arr[i])
      return 1;
  }
  return -1;
}

void addToSynch(int nonterminal_id,int terminal_id,Grammar g,int *visited,intLinkedList *keywordList)
{
  synch[nonterminal_id].arr[terminal_id] = 1;
  intNode *head = keywordList->head;
  while(head !=  NULL)
  {
  	synch[nonterminal_id].arr[head->data] = 1;
  	head = head->next;
  }
  int i;
  visited[nonterminal_id] = 1;
  for(i = Firstset[nonterminal_id].start;i <= Firstset[nonterminal_id].end;i++)
  {
    Rule r = g.rule[i];
    Node *rhs = r.rhs->head;
    while(rhs != NULL)
    {
        if(rhs->data.isNonTerminal && rhs->data.id != nonterminal_id && visited[rhs->data.id] != 1)
        {
            addToSynch(rhs->data.id,terminal_id,g,visited,keywordList);       
        }
        
        rhs = rhs->next;      
    }
  }
}

void setSynch(Grammar g)
{

  int i;
  int visited[lenNonTerminal];

  char **word = setKeyword();
  synch = (Synch *)malloc(sizeof(Synch)*lenNonTerminal);

  for(i = 0;i < lenNonTerminal;i++)
  {
    synch[i].arr = (int *)malloc(sizeof(int)*lenTerminal);
    int j;
    for(j = 0;j < lenTerminal;j++)
      synch[i].arr[j] = 0;
    strcpy(synch[i].name,Firstset[i].name);
    //[i].occur = getIntLinkedList();
  }

  //FollowSet[0].arr[56] = 1; // Follow of <program> is $.

    int j;
  for(i = 0;i < NonNullableRules;i++)
  {
    Rule r = g.rule[i];
    Node *rhs = r.rhs->tail;
    Node *previous;
    Symbol s,q;
    intLinkedList *keywordList ;
    keywordList = getIntLinkedList();
    while(rhs != NULL)
    {
      s = rhs->data;
      if(s.isNonTerminal && rhs->prev != NULL)
      {
          
          int o;
          
          for(o = Firstset[s.id].start;o <= Firstset[s.id].end;o++)
          {
            Rule y = g.rule[i];
            Node *w = y.rhs->head;
            while(w != NULL)
            {
              Symbol l = w->data;
              if(l.isNonTerminal == 0)
              {
                insertIntAtBack(keywordList,l.id);
              }
              w = w->next;
            }
          }
      }
      else if(s.isNonTerminal == 0 && checkInSynch(s.id,word) >= 0)
      {
        previous = rhs->prev;
         while(previous != NULL)
         {

            q = previous->data;
            if(q.isNonTerminal)
            {
                memset(visited,0,sizeof(visited));
                addToSynch(q.id,s.id,g,visited,keywordList);
            }
            previous = previous->prev; 
         }
      }
      rhs = rhs->prev;
    }
  }

  // FILE *fp = fopen("synch.txt","w");
  //   for(i = 0;i < lenNonTerminal;i++)
  //   {
  //       fprintf(fp,"%s -> ",synch[i].name );
  //       for(j = 0;j < lenTerminal;j++)
  //       {
  //           if(synch[i].arr[j] == 0)
  //               continue;
  //           fprintf(fp, "%s ", hashTerminal[j]);        
  //       } 
  //       fprintf(fp, "%s",".\n" );   
  //   }
  //   fclose(fp);
}  


//////////////////////        Synch End            ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void stack_simulate(FILE *fp,FILE *fp_error,FILE *fp_comment, int** parsetable, Grammar g)
{
  Stack* stack=initialize_stack();
  
  //FILE* fp_stack = fopen("stackout.txt", "w");

  int flag = 0;
  TOKEN_INFO* token = getNextToken(fp,fp_error,fp_comment,0),*prev_token = NULL;
  prev_token = malloc(sizeof(TOKEN_INFO));
  prev_token->token_id = malloc(sizeof(char)*100);
  prev_token->token_name = malloc(sizeof(char)*100);

  Stack_node *popped_symbol = stack->tail;
  int ruleno;

  while(strcmp(popped_symbol->data.name,"$") != 0)
  {
    //fflush(fp_stack);
    //fprintf(fp_stack,"Token %d %s %s\n",token->line_number,token->token_name,token->token_id );
    //print_stack(stack,fp_stack);

    // if(flag == 0)
    popped_symbol = pop_stack(stack);

    if(popped_symbol->ParseTreeNode != NULL)
    {
      popped_symbol->ParseTreeNode->token = token;
     // if(popped_symbol->ParseTreeNode->parent != NULL)
       // printf("parent %s\n",popped_symbol->ParseTreeNode->parent->data.name);
    }

    if(!popped_symbol->data.isNonTerminal || strcmp(popped_symbol->data.name,"$") == 0)
    {
      flag  = 0;
      if(strcmp(popped_symbol->data.name,"$") == 0){
       // printf("Compiled !!\n");
        // Why not break;
        }
      if(strcmp(token->token_id,popped_symbol->data.name) ==0){
        prev_token->line_number = token->line_number;
        strcpy(prev_token->token_name,token->token_name);
        strcpy(prev_token->token_id,token->token_id);
        
        token = getNextToken(fp,fp_error,fp_comment,0);
      }
      else{
        if(strcmp(token->token_name,"$") == 0)
        {
        if(print_syntax_error)
            fprintf(stdout,"ERROR_4: Input is consumed while it is expected to have token <%s> at line number <%d>\n",
               popped_symbol->data.name,
               token->line_number);
            error1 = 1;
        }
        else
        {
        if(print_syntax_error)
            fprintf(stdout,"ERROR_5: The token <%s> for lexeme <%s> does not match at line <%d>. The expected token here is <%s>\n",
            token->token_id,token->token_name,token->line_number,popped_symbol->data.name);
            error1 = 1;
        }
      }
    }
    else if((ruleno= parsetable[popped_symbol->data.id][checkInHash(token->token_id,1)]) != -1)
    {
      
      flag = 0;
      LinkedList *x = g.rule[ruleno].rhs;
      popped_symbol->ParseTreeNode->ruleno = ruleno;

      if( x==NULL)
      {
        
        ParseTreeNode* newnode = getTreeNode();
        Symbol s;
        strcpy(s.name,"epsilon");
        s.id = 58;
        s.isNonTerminal = 1;

      //  printf("epsilon dfsddfsdfssfsfd %s\n",newnode->token->token_name);
        newnode->data=s;
        newnode->parent = popped_symbol->ParseTreeNode;

        TreeLinkedList *x=insertChildTree(s,popped_symbol->ParseTreeNode);
        x->head->data->token = token;
        popped_symbol->ParseTreeNode->child = x;
      }

      Node *n=NULL;
      if(x!=NULL)
      {
        n=x->tail;
      }
      while(n!=NULL)
      {
        ParseTreeNode* newnode = getTreeNode();
        newnode->data=n->data;
        newnode->parent = popped_symbol->ParseTreeNode;

        TreeLinkedList *x = insertChildTree(n->data,popped_symbol->ParseTreeNode);
        popped_symbol->ParseTreeNode->child = x;

        push_stack(stack,n->data,popped_symbol->ParseTreeNode->child->tail->data);

        n=n->prev;

      }
    }
    else
    {
      if(FollowSet[popped_symbol->data.id].arr[checkInHash(token->token_id,1)] == 1 || synch[popped_symbol->data.id].arr[checkInHash(token->token_id,1)] == 1)
      {
        flag = 0;
        // fprintf(fp_error,"parsing error 2 %d %s %s %s\n",
        //        token->line_number,
        //        token->token_name,
        //        token->token_id,
        //        popped_symbol->data.name);
        if(print_syntax_error)
        fprintf(stdout,"ERROR_5: The token <%s> for lexeme <%s> does not match at line <%d>. The expected token here is of type <%s>\n",
            token->token_id,token->token_name,token->line_number,popped_symbol->data.name);
            error1 = 1;
        continue;
      }
      else
      {
        prev_token->line_number = token->line_number;
        strcpy(prev_token->token_name,token->token_name);
        strcpy(prev_token->token_id,token->token_id);

        token = getNextToken(fp, fp_error, fp_comment,0);
        if(strcmp(prev_token->token_id,token->token_id) == 0 
          && strcmp(prev_token->token_id,"$") == 0)
        {
	if(print_syntax_error)
            fprintf(stdout,"ERROR_4: Input is consumed while it is expected to have token of type <%s> at line number <%d>\n",
               popped_symbol->data.name,
               token->line_number);
            error1 = 1;

            TOKEN_INFO *t = malloc(sizeof(TOKEN_INFO));
            t->line_number = -1;
            t->token_id = malloc(sizeof(char)*10);
            t->token_name = malloc(sizeof(char)*10);
            // Empty Stack
            while(1)
            {
                if(strcmp(popped_symbol->data.name,"$") == 0)
                    break;
                popped_symbol->ParseTreeNode->token = t;
                popped_symbol = pop_stack(stack);
            }
          
            break;
        }
        // flag  = 1;
        //There could be a problem in construction of ParseTree

        push_stack(stack,popped_symbol->data,popped_symbol->ParseTreeNode);
        if(print_syntax_error)
        fprintf(stdout,"ERROR_5: The token <%s> for lexeme <%s> does not match at line <%d>. The expected token here is of type <%s>\n",
            token->token_id,token->token_name,token->line_number,popped_symbol->data.name);
            error1 = 1;
      }
    }
  }
//  fclose(fp);
//  fclose(fp_error);
//  fclose(fp_comment);
//  fclose(fp_stack);
}

int check_number(ParseTreeNode* root)
{
  char *node_name = root->data.name;

  if(strcmp(node_name,"TK_NUM") == 0 || strcmp(node_name,"TK_RNUM") == 0)
    return 1;

  return 0;
}


int printDFSTree(ParseTreeNode* root,int flag)
{
  if(root == NULL)
    return 0;
  int nodes = 0;
  TreeLinkedList* tree;
  tree = root->child;

 
   // fprintf(fp, "%-25s %-5d %-15s %-7s %-25s %-7s %-15s\n"
   //    , (root->data.isNonTerminal? "---":root->token->token_name)
   //    , root->token->line_number
   //    , root->token->token_id
   //    , (!root->data.isNonTerminal && check_number(root)? root->token->token_name:"---")
   //    , (root->parent? root->parent->data.name:"ROOT")
   //    , (!root->data.isNonTerminal ? "yes":"no")
   //    , (root->data.isNonTerminal? root->data.name:"---"));

  if(flag){
    if(root->token != NULL)
    printf( "%-25s %-5d %-15s %-7s %-25s %-7s %-15s\n"
      , (root->data.isNonTerminal? "---":root->token->token_name)
      , root->token->line_number
      , root->token->token_id
      , (!root->data.isNonTerminal && check_number(root)? root->token->token_name:"---")
      , (root->parent? root->parent->data.name:"ROOT")
      , (!root->data.isNonTerminal ? "yes":"no")
      , (root->data.isNonTerminal? root->data.name:"---"));
    else
      printf( "%-25s %-5s %-15s %-7s %-25s %-7s %-15s\n"
      , "---"
      , "---"
      , "---"
      , "---"
      , "---"
      , "---"
      , "---");
  }


  if(tree == NULL || tree->head == NULL)
    return 1;

  treeListNode* head = tree->tail;
  nodes = 1;
  while(head != NULL)
  {
    nodes += printDFSTree(head->data,flag);
    
    head = head->prev;
  }
  return nodes;
}


