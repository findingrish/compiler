// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "typeCheck.h"
#include "codeGen.h"

int reg[4];
int labelindex;
extern void codeStmt(Ast *root,symbolTable *st,FILE *fp);
void codetypedef(Ast *tree,symbolTable *st,FILE *fp)
{
	if(tree == NULL)
		return;
	declarationNode *d = st->globals;
	while(d)
	{
		fprintf(fp, "\t%s%s%d\n",variableindex[d->name],"  resb  ",4*(d->width));
		d = d->next;
	}
	d = st->functions->declaration;
	while(d)
	{
		fprintf(fp, "\t%s%s%d\n",variableindex[d->name],"  resb  ",4*(d->width));
		d = d->next;
	}
}

char* handlerecord(Ast *lchild,symbolTable *st,FILE *fp)
{
	char *s;
	s = malloc(sizeof(char)*20);
	strcpy(s,"[");
	strcat(s,lchild->token->token_name);
	recordNode *r = st->records;
	int a,b,offset,w;
	w = findSymbol("_main",lchild->token->token_name,"",st,0,0,0,0);
	a = getNumAfter(variableTrie,lchild->token->token_name);
	b = getNumAfter(variableTrie,lchild->child->token->token_name);

	while(r)
	{
		if(r->name == w)
		{
			declarationNode *d = r->fields;
			while(d)
			{
				if(d->name == b){
					offset = d->offset;
					// printf("%s %s\n",variableindex[b],variableindex[d->name]);
					break;
				}
				d = d->next;
			}
			break;
		}
		r = r->next;
	}
	offset = 4*offset;
	char c[5];
	strcpy(c,"+");
	strcat(s,c);

	sprintf(c,"%d",offset);
	strcat(s,c);
	
	strcat(s,"]");
	
	return s;
}


void initialize()
{
	reg[0] = 0;
	reg[1] = 0;
	reg[2] = 0;
	reg[3] = 0;
}

int getfree()
{
	int i;
	for(i = 0;i < 4;i++)
		if(reg[i] == 0)
			return i;
	return -1;
}


char *getregName(int x)
{
	char *s;
	s = malloc(5*sizeof(char));
	strcpy(s,"r");
	s[strlen(s)] = 97+x;
	strcat(s,"x");
	return s;
}

void makefree(int x,FILE *fp)
{
	fprintf(fp, "\n%s%s%s%s\n","\txor ",getregName(x),",",getregName(x));
}

void handleop(char *op,FILE *fp,int flag,int left,Ast *lchild)
{
	if(strcmp(op,"TK_PLUS") == 0)
	{
		if(!flag)
			fprintf(fp, "%s%s\n\n","\tadd rax,","rbx");
		else
		{
			int i;
			for(i = 0;i < flag;i++)
			{
				fprintf(fp, "%s%d%s\n","\tmov rax,[temp0",i,"]");
				fprintf(fp, "%s%d%s\n","\tmov rbx,[temp1",i,"]");
				fprintf(fp, "%s\n","\tadd rax,rbx" );
				if(left == 0)
				{
					fprintf(fp, "%s%d%s\n\n","\tmov [temp0",i,"],rax\n\txor rax,rax");
				}
				else
				{
					fprintf(fp, "%s%d%s\n\n","\tmov [temp1",i,"],rax\n\txor rax,rax");
				}
			}	
		}
	}
	else if(strcmp(op,"TK_MINUS") == 0)
	{
		if(!flag)
			fprintf(fp, "%s%s\n\n","\tsub rax,","rbx");
		else
		{
			int i;
			for(i = 0;i < flag;i++)
			{
				fprintf(fp, "%s%d%s\n","\tmov rax,[temp0",i,"]");
				fprintf(fp, "%s%d%s\n","\tmov rbx,[temp1",i,"]");
				fprintf(fp, "%s\n","\tsub rax,rbx" );
				if(left == 0)
				{
					fprintf(fp, "%s%d%s\n\n","\tmov [temp0",i,"],rax\n\txor rax,rax");
				}
				else
				{
					fprintf(fp, "%s%d%s\n\n","\tmov [temp1",i,"],rax\n\txor rax,rax");
				}
			}	
		}
	}
	else if(strcmp(op,"TK_MUL") == 0)
	{
		// add code for cases where register other than rax is used
		if(!flag)
			fprintf(fp, "%s%s\n\n","\tmul ","rbx");
		else
		{	
			int i;
			// the left child is a number
			if(lchild->type == 0)
			{
				for(i = 0;i < flag;i++)
				{
					// save rax
					fprintf(fp, "%s\n","\tmov rbx,rax\n\tpush rbx" );
					fprintf(fp, "%s%d%s\n","\tmov rbx,[temp1",i,"]");

					fprintf(fp, "%s\n","\tmul rbx" );
					if(left  == 0)
					{
						fprintf(fp, "%s%d%s\n\n","\tmov [temp0",i,"],rax");
					}
					else
					{
						fprintf(fp, "%s%d%s\n\n","\tmov [temp1",i,"],rax");
					}
					// reload rax
					fprintf(fp, "%s\n","\tpop rbx\n\tmov rax,rbx" );
				}
				fprintf(fp, "%s\n","\txor rax,rax\n\txor rbx,rbx" );					
			}
			else if(lchild->type != 0)
			{
				for(i = 0;i < flag;i++)
				{
				
					fprintf(fp, "%s%d%s\n","\tmov rax,[temp0",i,"]");
					fprintf(fp, "%s\n","\tmul rbx" );
					if(left  == 0)
					{
						fprintf(fp, "%s%d%s\n\n","\tmov [temp0",i,"],rax\n\txor rax,rax");
					}
					else
					{
						fprintf(fp, "%s%d%s\n\n","\tmov [temp1",i,"],rax\n\txor rax,rax");
					}
				}
			}
		}		
	}
	else if(strcmp(op,"TK_DIV") == 0)
	{
		fprintf(fp, "%s\n","\tpush rdx" );
		fprintf(fp, "%s\n","\txor rdx,rdx" );
		if(!flag)
			fprintf(fp, "%s%s\n","\tdiv ","rbx");
		else
		{	
			int i;
			// the left child is a number
			
				for(i = 0;i < flag;i++)
				{
				
					if(left  == 0)
					{
						fprintf(fp, "%s\n","\tpush rdx" );
						fprintf(fp, "%s\n","\txor rdx,rdx" );
						fprintf(fp, "%s%d%s\n","\tmov rax,[temp0",i,"]");
						fprintf(fp, "%s\n","\tdiv rbx" );
						fprintf(fp, "%s%d%s\n\n","\tmov [temp0",i,"],rax\n\txor rax,rax");
						fprintf(fp, "%s\n\n","\tpop rdx" );
					}
					else
					{
						fprintf(fp, "%s\n","\tpush rdx" );
		                fprintf(fp, "%s\n","\txor rdx,rdx" );
						fprintf(fp, "%s%d%s\n","\tmov rax,[temp0",i,"]");
						fprintf(fp, "%s\n","\tdiv rbx" );
						fprintf(fp, "%s%d%s\n\n","\tmov [temp1",i,"],rax\n\txor rax,rax ");
						fprintf(fp, "%s\n\n","\tpop rdx" );

					}
				}					
		}	
		fprintf(fp, "%s\n\n","\tpop rdx" );
	
	}
}

int codearithemetic(int left,Ast *root,symbolTable *st,FILE *fp)
{
	int freer,freel;
	Ast *lchild,*rchild;
	// add case for record addition
	if(strcmp(root->data.name,"TK_MUL")!= 0 && strcmp(root->data.name,"TK_DIV")!= 0 &&
		strcmp(root->data.name,"TK_PLUS")!= 0 && strcmp(root->data.name,"TK_MINUS")!= 0)
	{
		if(strcmp(root->data.name,"TK_NUM") == 0)
		{
			fprintf(fp, "%s%s%s%s\n","\tmov ","rax",",",root->token->token_name);
			return 0;
		}
		else if(root->child)
		{
			fprintf(fp, "%s%s%s%s\n","\tmov ","rax",",",handlerecord(root,st,fp));	
			return 0;
		}
		else
		{
			// add case for record addition
			int a = findSymbol("_main",root->token->token_name,"",st,0,0,0,0);
			int b = getNumAfter(variableTrie,root->token->token_name);
			if(a == 0)
			{
				fprintf(fp, "%s%s%s%s%s%s\n","\tmov ","rax",",","[",root->token->token_name,"]");
				return 0;
			}
			else
			{
				recordNode *rec = st->records;
				//printf("%s %s %d %d\n",variableindex[a],variableindex[rec->name],a,rec->name);
				while(rec && rec->name != a)
				{
					rec = rec->next;
				}
				declarationNode *dec = rec->fields;
				int offset = 0;
				int t = 0;
				while(dec)
				{
					fprintf(fp, "%s%s%s%d%s\n","\tmov rax,[",root->token->token_name,"+",offset,"]"); 
					fprintf(fp, "%s%d%d%s\n\n","\tmov [temp",left,t,"],rax");
					t++;  
					offset += 8;
					dec = dec->next;		
				}
				return t;
			}
			//fprintf(fp, "%s%s%s%s%s%s\n","\tmov ","rax",",","[",root->token->token_name,"]");
		}
	}
	else
	{

		lchild = root->child;
		rchild = root->child->next;
		// printf("root is %s\n",root->data.name);
		// printf("lchild is %s\n",lchild->data.name);
		// printf("rchild is %s\n",rchild->data.name);
		 int f1,f2;
		f1 = codearithemetic(1,rchild,st,fp);
		//printf("f1 is %d\n",f1);
		if(((lchild->type == 0) ||(lchild->type == 1)) || ((rchild->type == 0) ||(rchild->type == 1))  )
			fprintf(fp, "%s\n","\tmov rbx,rax\n\tpush rbx" );
		f2 = codearithemetic(0,lchild,st,fp);
		if(((lchild->type == 0) ||(lchild->type == 1)) || ((rchild->type == 0) ||(rchild->type == 1))  )
			fprintf(fp, "%s\n\n","\tpop rbx" );
		fprintf(fp, "%s%s %s %s\n\n","; arith op, root left right ",root->data.name,lchild->token->token_name
		,rchild->token->token_name );
		if(f1 == 0 && f2 >= 2)
			f1 = f2;
		handleop(root->data.name,fp,f1,left,lchild);
		return f1;
	}		
}



void codeassignop(Ast *root,symbolTable *st,FILE *fp)
{
	//fprintf(fp, "%s\n","\tpush rax\n\tpush rbx\n\tpush rcx\n\tpush rdx\n" );
	initialize();
	Ast *lchild = root->child,*rchild = root->child->next;
	if(strcmp(rchild->data.name,"TK_NUM") == 0)
	{
		if(lchild->child)
		{
			char *s = handlerecord(lchild,st,fp);
			makefree(0,fp);
			fprintf(fp, "%s%s\n","\tmov rax,",rchild->token->token_name );
			fprintf(fp, "%s%s%s%s\n","\tmov ",s,",","rax");
		}
		else
		{
			fprintf(fp, "%s%s\n","\tmov rax,",rchild->token->token_name );
			fprintf(fp, "%s%s%s%s\n" ,"\tmov [",lchild->token->token_name,"],","rax");
		}
	}
	else
	{
		// check if initialization required
		
		codearithemetic(0,rchild,st,fp);
		if(lchild->child)
		{
			char *s = handlerecord(lchild,st,fp);
			makefree(0,fp);
			fprintf(fp, "%s%s%s%s\n","\tmov ",s,",","rax");
		}
		else
		{
			//fprintf(fp, "%s%s%s%s\n" ,"\tmov [",lchild->token->token_name,"],","rax");
			int a = findSymbol("_main",lchild->token->token_name,"",st,0,0,0,0);
			//int b = getNumAfter(variableTrie,root->token->token_name);
			if(a == 0)
			{
				//fprintf(fp, "%s%s%s%s%s%s\n","\tmov ","rax",",","[",root->token->token_name,"]");
				fprintf(fp, "%s%s%s%s\n" ,"\tmov [",lchild->token->token_name,"],","rax");
				//return 0;
			}
			else
			{
				recordNode *rec = st->records;
				//printf("%s %s %d %d\n",variableindex[a],variableindex[rec->name],a,rec->name);
				while(rec && rec->name != a)
				{
					rec = rec->next;
				}
				declarationNode *dec = rec->fields;
				int offset = 0;
				int t = 0;
				while(dec)
				{
					fprintf(fp, "%s%d%s\n","\tmov rax,[temp0",t,"]");
					fprintf(fp, "%s%s%s%d%s%s\n" ,"\tmov [",lchild->token->token_name,"+",offset,"],","rax");
					t++;  
					offset += 8;
					dec = dec->next;		
				}
				
			}
		}		
		
	}

	makefree(0,fp);
	//fprintf(fp, "%s\n","\tpop rax\n\tpop rbx\n\tpop rcx\n\tpop rdx\n" );
}


char* getlabel(int labelindex)
{
	char *l1,l2[10];
	l1 = malloc(sizeof(char)*10);
	strcpy(l1,"label:");
	sprintf(l2,"%d",labelindex);
	strcat(l1,l2);
	return l1;	
}

void labelboolean(Ast *root)
{
	if(strcmp(root->data.name,"TK_NOT") == 0)
	{
		labelboolean(root->child);		
	}
	else if(strcmp(root->data.name,"TK_OR")==0 || strcmp(root->data.name,"TK_AND")==0)
	{
		labelboolean(root->child);
		root->labelindex = labelindex;
		labelboolean(root->child->next);	
	}
	else
	{                                 	                                                                                                   
		root->labelindex = labelindex;
		labelindex++;
	}		
}
void notboolean(Ast *root,int flag)
{
	if(root == NULL)
		return;
	else if(strcmp(root->data.name,"TK_AND") == 0)
	{
		if(flag)
			strcpy(root->data.name,"TK_OR");
		notboolean(root->child,flag);
		notboolean(root->child->next,flag);	
	}
	else if(strcmp(root->data.name,"TK_OR") == 0)
	{
		if(flag)
			strcpy(root->data.name,"TK_AND");
		notboolean(root->child,flag);
		notboolean(root->child->next,flag);		
	}
	else if(strcmp(root->data.name,"TK_LT") == 0)
	{
		if(flag)
			strcpy(root->data.name,"TK_GE");
			
	}
	else if(strcmp(root->data.name,"TK_GT") == 0)
	{
		if(flag)
			strcpy(root->data.name,"TK_LE");
		
	}
	else if(strcmp(root->data.name,"TK_LE") == 0)
	{
		if(flag)
			strcpy(root->data.name,"TK_GT");
	
	}
	else if(strcmp(root->data.name,"TK_GE") == 0)
	{
		if(flag)
			strcpy(root->data.name,"TK_LT");
	
	}
	else if(strcmp(root->data.name,"TK_NE") == 0)
	{
		if(flag)
			strcpy(root->data.name,"TK_EQ");
	
	}
	else if(strcmp(root->data.name,"TK_EQ") == 0)
	{
		if(flag)
			strcpy(root->data.name,"TK_NE");
	
	}
	else if(strcmp(root->data.name,"TK_NOT") == 0)
	{
		strcpy(root->data.name,"TK"); // change not as this not is no more useful
		flag = ~flag;
		notboolean(root->child,flag);
	}
	else
	{
		return;
	}		
}
void printcmp(Ast *root,symbolTable *st,FILE *fp,int label)
{
	fprintf(fp, "%s%d%s\n","label",label,":");
	Ast *lchild,*rchild;
	lchild = root->child;
	rchild = root->child->next;
	if(lchild->child)
	{
		char *s = handlerecord(lchild,st,fp);
		fprintf(fp, "%s%s\n","\tmov rax,",lchild->token->token_name );
	}
	else
	{
		if(strcmp(lchild->data.name,"TK_NUM") == 0)
			fprintf(fp, "%s%s\n","\tmov rax,",lchild->token->token_name );
		else
			fprintf(fp, "%s%s%s\n","\tmov rax,[",lchild->token->token_name,"]");
	}	
	if(rchild->child)
	{
		char *s = handlerecord(rchild,st,fp);
		fprintf(fp, "%s%s\n","\tmov rbx,",rchild->token->token_name );
	}
	else
	{
		if(strcmp(rchild->data.name,"TK_NUM") == 0)
			fprintf(fp, "%s%s\n","\tmov rbx,",rchild->token->token_name );
		else
			fprintf(fp, "%s%s%s\n","\tmov rbx,[",rchild->token->token_name,"]");			
	}	
	fprintf(fp, "%s\n","\tcmp rax,rbx" );
}

void handleboolean(Ast *root,symbolTable *st,FILE *fp,int label1,int label2,Ast **stack,int *stackchild,int top)
{
	if(strcmp(root->data.name,"TK_NOT") == 0)
	{
		root = root->child;
		notboolean(root,1);
		handleboolean(root,st,fp,label1,label2,stack,stackchild,top);
	}
	else if(strcmp(root->data.name,"TK") == 0)
	{
		root = root->child;
		handleboolean(root,st,fp,label1,label2,stack,stackchild,top);
	}	
	else if(strcmp(root->data.name,"TK_OR") == 0 || strcmp(root->data.name,"TK_AND") == 0) 
	{
		int z = (top);
		stack[z+1] = root;
		stackchild[z+1] = 0;
		handleboolean(root->child,st,fp,label1,label2,stack,stackchild,(z+1));
		stackchild[z+1] = 1;
		handleboolean(root->child->next,st,fp,label1,label2,stack,stackchild,z+1);
	}
	else
	{                                                                                                                                    
		if(strcmp(root->data.name,"TK_LT") == 0)
		{
			int flag = 0;
			int topstack = top;
			printcmp(root,st,fp,root->labelindex);
			if(topstack == -1)
			{
				fprintf(fp, "%s%d\n","\tjge label",label2);
			}
			else
			{
				if(strcmp(stack[topstack]->data.name,"TK_OR") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_AND") == 0 && 
							(stackchild[k] == 0))
						{
							fprintf(fp, "%s%d\n","\tjl label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tjl label",label1);
					}
					k = topstack;
					while(k >= 0)
					{
						if(stackchild[k] == 1)
							k--;
						else
							break;
					}
					if(k < 0)
						fprintf(fp, "%s%d\n","\tjge label",label2);	
					
				}
				else if(strcmp(stack[topstack]->data.name,"TK_AND") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_OR") == 0 
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tjge label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tjge label",label2);
					}
										
				}
			}

		}
		else if(strcmp(root->data.name,"TK_LE") == 0)
		{
			int topstack = top;
			printcmp(root,st,fp,root->labelindex);
			if(topstack == -1)
			{
				fprintf(fp, "%s%d\n","\tjg label",label2);
			}
			else
			{
				if(strcmp(stack[topstack]->data.name,"TK_OR") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_AND") == 0
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tjle label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tjle label",label1);
					}
					k = topstack;
					while(k >= 0)
					{
						if(stackchild[k] == 1)
							k--;
						else
							break;
					}
					if(k < 0)
						fprintf(fp, "%s%d\n","\tjg label",label2);	
				}
				else if(strcmp(stack[topstack]->data.name,"TK_AND") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_OR") == 0
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tjg label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tjg label",label2);
					}					
				}
			}			
		}
		else if(strcmp(root->data.name,"TK_GT") == 0)
		{
			int topstack = top;
			printcmp(root,st,fp,root->labelindex);
			if(topstack == -1)
			{
				fprintf(fp, "%s%d\n","\tjle label",label2);
			}
			else
			{
				if(strcmp(stack[topstack]->data.name,"TK_OR") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_AND") == 0
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tjg label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tjg label",label1);
					}
					k = topstack;
					while(k >= 0)
					{
						if(stackchild[k] == 1)
							k--;
						else
							break;
					}
					if(k < 0)
						fprintf(fp, "%s%d\n","\tjle label",label2);	
				}
				else if(strcmp(stack[topstack]->data.name,"TK_AND") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_OR") == 0
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tjle label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tjle label",label2);
					}					
				}
			}			
		}
		else if(strcmp(root->data.name,"TK_GE") == 0)
		{
			int topstack = top;
			printcmp(root,st,fp,root->labelindex);
			if(topstack == -1)
			{
				fprintf(fp, "%s%d\n","\tjl label",label2);
			}
			else
			{
				if(strcmp(stack[topstack]->data.name,"TK_OR") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_AND") == 0
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tjge label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tjge label",label1);
					}
					k = topstack;
					while(k >= 0)
					{
						if(stackchild[k] == 1)
							k--;
						else
							break;
					}
					if(k < 0)
						fprintf(fp, "%s%d\n","\tjl label",label2);	
				}
				else if(strcmp(stack[topstack]->data.name,"TK_AND") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_OR") == 0
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tjl label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tjl label",label2);
					}					
				}
			}			
		}
		else if(strcmp(root->data.name,"TK_EQ") == 0)
		{
			int topstack = top;
			printcmp(root,st,fp,root->labelindex);
			if(topstack == -1)
			{
				fprintf(fp, "%s%d\n","\tjne label",label2);
			}
			else
			{
				if(strcmp(stack[topstack]->data.name,"TK_OR") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_AND") == 0
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tje label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tje label",label1);
					}
					k = topstack;
					while(k >= 0)
					{
						if(stackchild[k] == 1)
							k--;
						else
							break;
					}
					if(k < 0)
						fprintf(fp, "%s%d\n","\tjne label",label2);	
				}
				else if(strcmp(stack[topstack]->data.name,"TK_AND") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_OR") == 0
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tjne label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tjne label",label2);
					}					
				}
			}			
		}
		else	// Not equal
		{
			int topstack = top;
			printcmp(root,st,fp,root->labelindex);
			if(topstack == -1)
			{
				fprintf(fp, "%s%d\n","\tje label",label2);
			}
			else
			{
				if(strcmp(stack[topstack]->data.name,"TK_OR") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_AND") == 0
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tjne label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tjne label",label1);
					}
					k = topstack;
					while(k >= 0)
					{
						if(stackchild[k] == 1)
							k--;
						else
							break;
					}
					if(k < 0)
						fprintf(fp, "%s%d\n","\tje label",label2);	
				}
				else if(strcmp(stack[topstack]->data.name,"TK_AND") == 0)
				{
					int k = topstack;
					while(k >= 0)
					{
						if(strcmp(stack[k]->data.name,"TK_OR") == 0
							&& stackchild[k] == 0)
						{
							fprintf(fp, "%s%d\n","\tje label",stack[k]->labelindex);
							break;
						}
						k--;
					}
					if(k < 0)
					{
						fprintf(fp, "%s%d\n","\tje label",label2);
					}					
				}
			}			
		}
	}
}

void codeconditional(Ast *root,symbolTable *st,FILE *fp)
{

	
	Ast *stack[100];
	int childstack[100];
	int top;
	top = -1;
	root = root->child;
	fprintf(fp, "%s%d\n","; starting label of if else ",labelindex );
	labelboolean(root);
	

	handleboolean(root,st,fp,labelindex,labelindex+1,stack,childstack,top);	
	
	fprintf(fp, "%s%d\n","; starting label of if part ",labelindex );
	fprintf(fp, "%s%d%s\n","label",labelindex,":");
	
	labelindex = labelindex+1;
	int elselabel = labelindex;
	labelindex = labelindex + 1;
		// reserve label for if there is else part
	int afterelselabel = labelindex;
	root = root->next;

	labelindex++;
	codeStmt(root->child,st,fp);
	
	if(root->next->child)
		fprintf(fp, "\t%s%d\n","jmp label",afterelselabel);
	
	fprintf(fp, "%s%d\n","; start of else part ",labelindex );
	fprintf(fp, "%s%d%s\n","label",elselabel,":");


	if(root->next->child){
		labelindex++;
		codeStmt(root->next->child,st,fp);
		
	}
	fprintf(fp, "%s%d\n","; end of if-else statement ",labelindex );
	fprintf(fp, "%s%d%s\n","label",afterelselabel,":");
	
	
	labelindex++;

}

void codeiterative(Ast *root,symbolTable *st,FILE *fp)
{

	
	Ast *stack[100];
	int childstack[100];
	int top;
	top = -1;
	root = root->child;
	int startlabel = labelindex;

	labelboolean(root);
	
	fprintf(fp, "\n%s%d\n","; starting label of while ",startlabel);

	handleboolean(root,st,fp,labelindex,labelindex+1,stack,childstack,top);	
	
	fprintf(fp, "\n%s%d\n","; starting label of while body ",labelindex);

	fprintf(fp, "%s%d%s\n\n","label",labelindex,":");
	labelindex++;
	root = root->next;
	// save labelindex+1
	int nextlabel = labelindex;
	labelindex++;
	codeStmt(root->child,st,fp);
	fprintf(fp, "\t%s%d\n\n","jmp label",startlabel);
	fprintf(fp, "\n%s%d\n","; end of while loop ",labelindex);
	fprintf(fp, "%s%d%s\n\n","label",nextlabel,":");

	
}

void codeiostmt(Ast *root,symbolTable *st,FILE *fp)
{
	root = root->child;
	if(strcmp(root->data.name,"TK_WRITE") == 0)
	{
		root = root->next;
		if(root->child)
		{
			fprintf(fp, "%s%s\n","\tmov rsi,",handlerecord(root,st,fp));  
			fprintf(fp, "%s\n","\tmov rdi,msg1\n\tsub rsp,8\n\tcall printf\n\tadd rsp,8\n");
		}
		else
		{
			int a = findSymbol("_main",root->token->token_name,"",st,0,0,0,0);
			int b = getNumAfter(variableTrie,root->token->token_name);
			if(a == 0)
			{
				fprintf(fp, "%s%s%s\n","\tmov rsi,[",root->token->token_name,"]");  
				fprintf(fp, "%s\n","\tmov rdi,msg1\n\tsub rsp,8\n\tcall printf\n\tadd rsp,8\n");
			}
			else
			{
				recordNode *rec = st->records;
				//printf("%s %s %d %d\n",variableindex[a],variableindex[rec->name],a,rec->name);
				while(rec && rec->name != a)
				{
					rec = rec->next;
				}
				declarationNode *dec = rec->fields;
				int offset = 0;
				while(dec)
				{
					fprintf(fp, "%s%s%s%d%s\n","\tmov rsi,[",root->token->token_name,"+",offset,"]");  
					fprintf(fp, "%s\n","\tmov rdi,msg1\n\tsub rsp,8\n\tcall printf\n\tadd rsp,8\n");
					offset += 8;
					dec = dec->next;		
				}
			}
		}
	}
	else
	{
		root = root->next;
		if(root->child)
		{
			char *s = handlerecord(root,st,fp);
			
			s[strlen(s)-1] = '\0';
			fprintf(fp, "%s%s\n","\tmov rsi,",s+1);  
			fprintf(fp, "%s\n","\tmov rdi,msg2\n\tsub rsp,8\n\tcall scanf\n\tadd rsp,8\n");
		}
		else
		{
			int a = findSymbol("_main",root->token->token_name,"",st,0,0,0,0);
			int b = getNumAfter(variableTrie,root->token->token_name);
			if(a == 0)
			{
				fprintf(fp, "%s%s\n","\tmov rsi,",root->token->token_name);  
				fprintf(fp, "%s\n","\tmov rdi,msg2\n\tsub rsp,8\n\tcall scanf\n\tadd rsp,8\n");
			}
			else
			{
				recordNode *rec = st->records;
				while(rec && rec->name != a)
				{
					rec = rec->next;
				}
				declarationNode *dec = rec->fields;
				int offset = 0;
				while(dec)
				{
					fprintf(fp, "%s%s%s%d\n","\tmov rsi,",root->token->token_name,"+",offset);  
					fprintf(fp, "%s\n","\tmov rdi,msg2\n\tsub rsp,8\n\tcall scanf\n\tadd rsp,8\n");
					offset += 8;
					dec = dec->next;		
				}
			}
		}
	}
}
void codeStmt(Ast *root,symbolTable *st,FILE *fp)
{
	if(root == NULL)
		return;

	while(root != NULL)
	{
		if(strcmp(root->data.name,"iterativeStmt") == 0)
			codeiterative(root,st,fp);
		else if(strcmp(root->data.name,"conditionalStmt") == 0)
			codeconditional(root,st,fp);
		else if(strcmp(root->data.name,"TK_ASSIGNOP")== 0)
			codeassignop(root,st,fp);
		else if(strcmp(root->data.name,"ioStmt") == 0)
			codeiostmt(root,st,fp);
		root = root->next;
	}
}
void code(Ast *tree,symbolTable *st,char *out)
{
	FILE *fp;
	labelindex = 0;
	fp = fopen(out,"w");
	tree = tree->child;
	while(tree->next)
		tree = tree->next;
	// tree point to mainfunction

	tree = tree->child->next;
	// tree is stmts
	tree = tree->child;
	//tree is typedef

	fprintf(fp, "%s%s","global main\n","extern printf\nextern scanf\n");
	
	//printfloat(fp);
	fprintf(fp, "%s\n","\nSECTION .bss\n");
	codetypedef(tree,st,fp);
	fprintf(fp, "%s%s\n","\nSECTION .text\n","main:\n");
	codeStmt(tree->next->next->child,st,fp);
	
	fprintf(fp, "%s\n","ret" );
	int i;
	fprintf(fp, "%s\n","\nSECTION .data\n");
	fprintf(fp, "%s\n","\tmsg1 db '%d',10,0 ");
	fprintf(fp, "%s\n","\tmsg2 db '%d',0 ");
	for(i = 0;i < 16;i++)
	{
		if(i < 8)
			fprintf(fp, "\t%s%d%s%d%s%d\n","temp0",i," dd ",0,",",0);
		else
			fprintf(fp, "\t%s%d%s%d%s%d\n","temp1",i-8," dd ",0,",",0);
	}
	
	fclose(fp);
	
}