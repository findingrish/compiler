// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"

// Constrcution for allocating memory
void* getNode(int a)
{
	void *z;
	if(a == 0) // declarationNode
	{
		declarationNode *x; 
		x = malloc(sizeof(declarationNode));
		x->next = NULL;
		x->name = 5;
		x->dataType = 0;
		x->offset = 0; 
		x->used = 0;
		x->iter = 0;
		z = (void *)x;
	}	
	else if(a == 1) // functionNode
	{
		functionNode *x;
		x = malloc(sizeof(functionNode));
		x->inputParam = NULL;
		x->outputParam = NULL;
		x->next = NULL;
		x->declaration = NULL;
		x->name = 0;
		z = (void *)x;
	}
	else if(a == 2)
	{
		recordNode *x;
		x = malloc(sizeof(recordNode)); // recordNode
		x->name = 0;
		x->width = 0;
		x->fields = NULL;
		x->next = NULL;
		z = (void *)x;
	}
	else
	{
		symbolTable *x;
		x = malloc(sizeof(symbolTable));
		x->records = NULL;
		x->functions = NULL;
		x->globals = NULL;
		z = (void *)x;
	}
	return z;
}

// Trie is built 
void insertTrie(trie *root,char *st,int id)
{
	int j ;
	int len = strlen(st),i;
	trie *head = root;
	trie *x;
	for(i = 0;i < len;i++)
	{
		if(head->child[st[i]-'#'] == NULL)
		{
		 	x = (trie *)malloc(sizeof(trie));
			for(j = 0;j < 123;j++)
			{
				x->child[j] = NULL;
				x->isleaf = 0;
				x->id = -1;
			}		
			
			head->child[st[i]-'#'] = x;
			head = head->child[st[i]-'#'];	 
		}
		else
		{
			head = head->child[st[i]-'#'];
		}
	}
	head->isleaf = 1;
	head->id = id;

}

int findTrie(trie *root,char *st)
{
	if(root == NULL)
		return -1;
	
	int len = strlen(st),i;

	for(i = 0;i < len;i++)
	{
		if(root->child[st[i]-'#'] == NULL)
			return -1;
		root = root->child[st[i]-'#'];
	}

  	if(root->isleaf)
		return root->id;
  	else
    	return -1; 
}


int getNum(trie *root,char *st)
{
	int x = findTrie(root,st);
	if(x == -1)
	{
		x = globalNum;
		if(globalNum >= variableindexsize)
		{
			variableindexsize = 2*variableindexsize;
			char **r = (char **)malloc(variableindexsize*sizeof(char *));
			int i;
			for(i = 0;i < globalNum;i++)
			{
				r[i] = (char *)malloc(strlen(variableindex[i])*sizeof(char));
				strcpy(r[i],variableindex[i]);
			}
			variableindex = r;
		}
		variableindex[globalNum] = (char *)malloc(strlen(st)*sizeof(char));
		strcpy(variableindex[globalNum],st);
		globalNum++;
		insertTrie(root,st,x);
	}
	return x;
}

int getNumAfter(trie *root,char *st)
{
	return findTrie(root,st);
}

void addRecords(Ast *root,symbolTable *st,FILE *fp)
{
	
	recordNode *r = getNode(2);
	root = root->child;
	
	if(root == NULL)
		return;

	int line = root->token->line_number;
	r->name = getNum(variableTrie,root->token->token_name);
	
	root = root->next->child;
	
	declarationNode *head,*d;
	
	
	int *visited = malloc(1000*sizeof(int));
	int index = 0,j,flag = 0;
	int start = 0;
	int name,dataType;
	while(root != NULL)
	{	

		name = getNum(variableTrie,root->child->token->token_name);
		dataType = getNum(variableTrie,root->token->token_name);
		
		for(j = 0;j < index;j++)
		{
			if(name == visited[j])
			{
				if(print_semantic_error)
					fprintf(stdout,"%s%d%s%s%s%s%s\n","Line ",root->token->line_number,
						" : variable <",variableindex[name],"> of Record <",
						variableindex[r->name],"> has Conflicting name");
				error2  =1;
				flag = 1;
				//printf("Hey\n");
				break;
			}
		}
		
		if(flag == 0)
		{
			if(start == 0)
			{
				head = getNode(0);
				d = head;
				start = 1;
			}
			else
			{
				d->next = getNode(0);
				d = d->next;
			}
			d->name = name;
			d->dataType = dataType;
			visited[index] = name;
			index++;
		}
		root = root->next;
		flag = 0;
	}
	free(visited);
	r->fields = head;
	
	if(st->records == NULL)
	{
		st->records = r;
	}
	else
	{
		recordNode *w;
		w = st->records;
		while(w->next != NULL)
		{
			if(w->name == r->name)
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s\n","Line ",line," : Record with name <",variableindex[w->name]
					,"> already exist" );
				error2  =1;
				return;
			}

			w = w->next;
		}
		if(w->name == r->name)
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s\n","Line ",line," : Record with name <",variableindex[w->name]
					,"> already exist" );
				error2  =1;
			}

		w->next = r;
	}
	
}

// Global variables are added to symbol table
void addGlobals(int name,int dataType,symbolTable *st,FILE *fp,functionNode *f,int line)
{
	declarationNode *x = getNode(0);
	x->name = name;
	x->dataType = dataType;
	x->line = line;
	if(st->globals == NULL)
		st->globals = x;
	else
	{
		declarationNode *y;
		y = st->globals;
	
		while(y->next != NULL)
		{
			if(y->name == name)
			{
				fprintf(stdout, "%s%d%s%s%s\n","Line ",line," : variable <",variableindex[name],
				"> being a global variable cannot be declared more than once");
				return;
			}
			y = y->next;
		}
		// Add condition for checking for variable in other function
		if(y->name == name)
		{
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s%s%s\n","Line ",line," : variable <",variableindex[name],
					"> being a global variable cannot be declared more than once");
			error2  =1;
			return;
		}
		y->next = x;
	}
	return;
}

// Declarations are checked and symbols extracted
declarationNode* addDeclaration(Ast *x,symbolTable *st,FILE *fp,functionNode *f,int iod)
{
	declarationNode *head,*d;
	d = NULL;
	head = NULL;
	int name,dataType;
	x = x->child; // declarations -> declaration declaration declaration .... 
	if(x == NULL)
	{
		head = NULL;
		return head;
	}
	int *visited = malloc(1000*sizeof(int));
	int index = 0,j,flag = 0,flag2 = 0;
	int start = 0;
	while(x != NULL)
	{
		name = getNum(variableTrie,x->child->token->token_name);
		dataType = getNum(variableTrie,x->token->token_name);
		
		if(x->child->child) // Implies Global 
		{
			addGlobals(name,dataType,st,fp,f,x->child->token->line_number);
			flag2 = 1;
		}
		else
		{
			//check for conflict with already declared variable inside function
			for(j = 0;j < index;j++)
			{
				if(name == visited[j])
				{
					if(print_semantic_error)
						fprintf(stdout,"%s%d%s%s%s%s%s\n","Line ",x->token->line_number,
							" : Variable <",variableindex[name],"> in function <",
							variableindex[f->name],"> has Conflicting Type");
					error2  =1;
					flag = 1;
					break;
				}
			}
			//check for conflict with with input parameters or output parameter
			if(iod == 3 && flag == 0)
			{
				declarationNode *input = f->inputParam;
				while(input)
				{
					if(input->name == name)
					{
					if(print_semantic_error)
						fprintf(stdout,"%s%d%s%s%s%s%s\n","Line ",x->token->line_number,
							" : Variable <",variableindex[name],"> in function <",
							variableindex[f->name],"> has Conflicting Type with input parameter");
					
					error2 = 1;
					flag = 1;
					break;
					}
					input = input->next;
				}
				input = f->outputParam;
				while(input)
				{
					if(input->name == name)
					{
					if(print_semantic_error)
						fprintf(stdout,"%s%d%s%s%s%s%s\n","Line ",x->token->line_number,
							" : Variable <",variableindex[name],"> in function <",
							variableindex[f->name],"> has Conflicting Type with output parameter");
					error2  =1;
					flag = 1;
					break;
					}
					input = input->next;
				}

			}
			//check for conflict with global variables ;

			// Keep all parameter of function even in case of conflict
			if(iod == 1 || iod == 2)
				flag = 0;
				
		}
		if(flag == 0 && flag2 == 0)
		{
			if(start == 0)
			{
				head = getNode(0);
				d = head;
				start = 1;
			}
			else
			{
				d->next = getNode(0);
				d = d->next;
			}
			d->name = name;
			d->dataType = dataType;
			d->line = x->token->line_number;
			visited[index] = name;
			index++;
		}
		x = x->next;
		flag2 = 0;
		flag = 0;
	}
	return head;
}

// All functions are added to symbol table
void addFunction(Ast *root,symbolTable *st,FILE *fp)
{
	functionNode *func;
	func = getNode(1);
	
	root = root->child;
	int line = root->token->line_number;
	func->name = getNum(variableTrie,root->token->token_name);
	// If the fnction is not a main function
	if(strcmp(root->token->token_name,"_main") != 0)
	{
		root = root->next;
		func->inputParam = addDeclaration(root,st,fp,func,1);
		root = root->next;
		func->outputParam = addDeclaration(root,st,fp,func,2); 	
	}
	root = root->next;
	// Now root is at stmts
	Ast *recordDef = root->child->child,*declarations = root->child->next;
	
	while(recordDef != NULL)
	{
		addRecords(recordDef,st,fp);
		recordDef = recordDef->next;
	}
	
	func->declaration = addDeclaration(declarations,st,fp,func,3);

	if(st->functions == NULL)
		st->functions = func;
	else
	{
		functionNode *f;
		f = st->functions;
		while(f->next != NULL)
		{
			if(func->name == f->name)
			{
				if(print_semantic_error)
					fprintf(stdout,"%s%d%s%s%s\n","Line ",line,
						" : Function with name <",variableindex[func->name],"()> already exist");
				error2  =1;
				return;
			}
			f = f->next;
		}
		f->next = func;
	}
	return ;
}

// offset is decided for symbol table
int offsetdec(symbolTable *st,declarationNode *d,FILE *fp,int startoffset,functionNode *f)
{

		while(d != NULL)
		{
			d->offset = startoffset;
			if(d->dataType == 0 || d->dataType == 1)
			{
				d->width = d->dataType? realwidth:intwidth;
			}
			else
			{
				recordNode *rec = st->records;
			 	while(rec && rec->name != d->dataType)
		 			rec = rec->next;
		 		if(rec)
		 			d->width = rec->width;
		 		else
		 		{
		 			d->width = 0;
		 			if(print_semantic_error)
			 			fprintf(stdout, "%s%d%s%s%s%s%s%s%s\n","Line ",d->line," : Record type ",
			 				variableindex[d->dataType]," for varaible ",variableindex[d->name],
			 				" in function ",variableindex[f->name]," does not exist");
			 			error2  =1;
		 		}
			}
			startoffset += d->width;
			d = d->next;
		}
return startoffset;
}



void offset(symbolTable *st,FILE *fp)
{
	
	recordNode *r = st->records;

	// Handle records
	while(r != NULL)
	{
		declarationNode *d = r->fields;
	
		while(d != NULL)
		{
			d->offset = r->width;
			r->width += d->dataType? realwidth:intwidth;
			d = d->next;
		}
		r = r->next;

	}
	// handle globals
	declarationNode *d = st->globals;
	int startoffset = 0;
	while(d != NULL)
	{
		d->offset = startoffset;
		if(d->dataType == 0 || d->dataType == 1)
		{
			d->width = d->dataType? realwidth:intwidth;
		}
		else
		{
			recordNode *rec = st->records;
		 	while(rec->name != d->dataType)
	 			rec = rec->next;
	 		d->width = rec->width;
		}
		startoffset += d->width;
		d = d->next;
	}
	// Handle functions
	functionNode *f = st->functions;
	startoffset = 0;
	int a,b,c;
	while(f != NULL)
	{
		startoffset = 0;
		// Handle declarations
		a = offsetdec(st,f->inputParam,fp,startoffset,f);
		b = offsetdec(st,f->outputParam,fp,a,f);
		offsetdec(st,f->declaration,fp,b,f);
		f = f->next;
	}
}

// Returns the record name for a variable
char *getrecordtype(symbolTable *st,int recordname)
{
	char *s;
	s =  malloc(sizeof(char)*100);

	recordNode *r = st->records;
	while(r && r->name != recordname)
	{
		r = r->next;
	}
	
	declarationNode *x;
	x = NULL;
	if(r == NULL)
	{
		if(print_semantic_error)
			fprintf(stdout, "%s%s%s\n","Record with name <",variableindex[recordname],"> does not exist");
		error2 = 1;	
		strcpy(s,"");
		return s;
	}
	else
		x = r->fields;
	
	if(x == NULL)
	{
		strcpy(s,"");
		return s;
	}
	while(x->next != NULL)
	{
		strcat(s,variableindex[x->dataType]);
		strcat(s," x ");
		x = x->next;		
	}
	if(x)
	{
		strcat(s,variableindex[x->dataType]);	
	}
	
	return s;
}



void printSymbolTable(symbolTable *st)
{
	//FILE *fp;
	
	//fp = fopen("symbolTable.txt","w");
	fprintf(stdout, "%-12s%-30s%-15s%-7s\n\n","lexeme","type","scope","offset");
	declarationNode *g = st->globals;
	
	while(g != NULL)
	{
		if(g->dataType <= 1)
			fprintf(stdout, "%-12s%-30s%-15s%-7s\n\n", variableindex[g->name],variableindex[g->dataType],"global",
				"-");
		else
		{
			fprintf(stdout, "%-12s%-30s%-15s%-7s\n\n", variableindex[g->name],
				getrecordtype(st,g->dataType),"global"
				,"-");
		}
		g = g->next;
		
	}


	functionNode *f = st->functions;
	while(f != NULL)
	{
		
		declarationNode *x;
		x = f->inputParam;
		while(x != NULL)
		{
			if(x->dataType <= 1)
			fprintf(stdout, "%-12s%-30s%-15s%-7d\n\n",variableindex[x->name],variableindex[x->dataType],
				variableindex[f->name],x->offset);
			else
			{
				if(strcmp(getrecordtype(st,x->dataType),"") != 0)
					fprintf(stdout, "%-12s%-30s%-15s%-7d\n\n",variableindex[x->name],getrecordtype(st,x->dataType),
					variableindex[f->name],x->offset);
			}
			x = x->next;		
		}
	
		x = f->outputParam;
		while(x != NULL)
		{
			if(x->dataType <= 1)
			fprintf(stdout, "%-12s%-30s%-15s%-7d\n\n",variableindex[x->name],variableindex[x->dataType],
				variableindex[f->name],x->offset);
			else
			{
				if(strcmp(getrecordtype(st,x->dataType),"") != 0)
					fprintf(stdout, "%-12s%-30s%-15s%-7d\n\n",variableindex[x->name],getrecordtype(st,x->dataType),
					variableindex[f->name],x->offset);
			}
			x = x->next;		
		}
	
		x = f->declaration;
		while(x != NULL)
		{
			if(x->dataType <= 1)
			fprintf(stdout, "%-12s%-30s%-15s%-7d\n\n",variableindex[x->name],variableindex[x->dataType],
				variableindex[f->name],x->offset);
			else
			{
				if(strcmp(getrecordtype(st,x->dataType),"") != 0)
					fprintf(stdout, "%-12s%-30s%-15s%-7d\n\n",variableindex[x->name],getrecordtype(st,x->dataType),
					variableindex[f->name],x->offset);
			}
			x = x->next;		
		}
		f = f->next;
	}
	//fclose(fp);
}


symbolTable* populateSymbolTable(Ast *root,int flag)
{
	globalNum = 0;
	intwidth = 2;
	realwidth = 4;
	variableTrie = malloc(sizeof(trie));
	// Initialize trie
	int y;
	variableTrie->isleaf = 0;
		variableTrie->id = -1;
	for(y = 0;y < 123;y++)
	{
		variableTrie->child[y] = NULL;
		
	}	

	variableindex = (char **)malloc(200*sizeof(char *));
	variableindexsize = 200;
	
	getNum(variableTrie,"int");
	getNum(variableTrie,"real");
	getNum(variableTrie,"boolean");

	symbolTable *symbolTable;
	symbolTable = getNode(3);
	root = root->child;
	FILE *fp_sem_error;
	// = fopen("semantic_errors.txt","w");
	while(root != NULL)
	{
		addFunction(root,symbolTable,fp_sem_error);
		root = root->next;
	}
	
	offset(symbolTable,fp_sem_error);
	//fclose(fp_sem_error);
	if(flag)
		printSymbolTable(symbolTable);
	return symbolTable;

}


	// printf("%d\n",getNum(variableTrie,"_main"));
	// printf("%d\n",getNum(variableTrie,"_recordDemo1"));
	// printf("%d\n",getNum(variableTrie,"#cook"));
	// printf("%d\n",getNum(variableTrie,"b2b345"));
	// printf("%d\n",getNum(variableTrie,"b0b02"));
	// printf("%d\n",getNum(variableTrie,"b2b345"));
	// printf("%d\n",getNum(variableTrie,"_main"));
	// printf("%d\n",getNum(variableTrie,"#bfgfg"));
	// printf("%d\n",getNum(variableTrie,"#cook"));
