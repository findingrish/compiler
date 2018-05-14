// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "typeCheck.h"


extern void checkStmt(Ast *root,symbolTable *st,char *funName,FILE *fp,int iter);
// int is 0 real is 1 boolean 2
// Finds the type of symbol from symbol table
int findSymbol(char *function,char *symbol,char *field,symbolTable *st,int setused,int iter1,int iter2,int checkiter)
{
	if(st == NULL)
		return -1;
	int a,b,c,dataType = -1;
	a = getNumAfter(variableTrie,function);
	b = getNumAfter(variableTrie,symbol);
	c = getNumAfter(variableTrie,field);
	
	if(a == -1 || b == -1)
		return -1;
	
	if(c == -1 && strcmp(field,"") != 0)
		return -1;

	// First find in globals
	declarationNode *g = st->globals;
	
	while(g && g->name != b){
		g = g->next;
	}

	if(g){
		dataType = g->dataType;
		if(iter1)
			g->iter = 1;
		if(iter2)
			g->iter = 0;
		if(checkiter && g->iter)
			assigniter++;
	}


	functionNode *f = st->functions;
	// if varaible is not a global variable
	if(dataType == -1)
	{
		while(f && f->name != a)
			f = f->next;

		declarationNode *d;
		d = NULL;
		if(f)
			d = f->inputParam;
		while(d && (d->name != b))
			d = d->next;
		if(d){
			dataType = d->dataType;
			if(iter1)
				d->iter = 1;
			if(iter2)
				d->iter = 0;
			if(checkiter && d->iter)
				assigniter++;
		}
		d=  NULL;
		if(f)
		d = f->outputParam;
		while(d && (d->name != b))
			d = d->next;

		if(d){
			dataType = d->dataType;
			if(setused){
				d->used = 1;
			}
			if(iter1)
				d->iter = 1;
			if(iter2)
				d->iter = 0;
			
			if(checkiter && d->iter)
				assigniter++;
		}
		d = NULL;
		if(f)
		d = f->declaration;
	
		while(d && (d->name != b))
			d = d->next;
		
		if(d){
			dataType = d->dataType;
			if(iter1)
				d->iter = 1;
			if(iter2)
				d->iter = 0;
			if(checkiter && d->iter)
				assigniter++;
		}
	
	}

	if(strcmp(field,"") == 0)
		return dataType;
	else
	{

		recordNode *rec = st->records;
		while(rec && rec->name != dataType)
			rec = rec->next;
		dataType = -1;
		declarationNode *d;
		d = NULL;

		if(rec)
			d = rec->fields;
		
		while(d && d->name != c)
			d = d->next;
		
		if(d)
			dataType = d->dataType;
		
		return dataType;
	}
}


void install(Ast *root)
{
	root->isfloat = 1;
	root->index = floatarrindex;
	strcpy(floatarr[floatarrindex].value,root->token->token_name);

	strcpy(floatarr[floatarrindex].name,"float");
	char b[5];
	sprintf(b,"%d",floatarrindex);
	strcat(floatarr[floatarrindex].name,b);
	floatarrindex++;	
}


// Checks a arithemetic expression
void checkArithmetic(Ast *root,symbolTable *st,char *funName,FILE *fp)
{
	if(root == NULL )
		return;
	if(strcmp(root->data.name,"TK_NUM") == 0)
	{
		root->type = 0;
	}
	else if(strcmp(root->data.name,"TK_RNUM") == 0)
	{
		// If a float encountered push in floatarr .
		install(root);
		root->type = 1;
	}
	else if(strcmp(root->data.name,"TK_ID") == 0)
	{
		// Implies it of type say b2.edition
		if(root->child)
		{
			root->type = findSymbol(funName,root->token->token_name,root->child->token->token_name,st,0,0,0,0);	
			if(root->type == -1)
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s%s%s\n", "Line ",root->token->line_number
					," : variable <",root->token->token_name,".",
					root->child->token->token_name,"> is undeclared");
				error2 = 1;
			}	
		}
		else
		{
			root->type = findSymbol(funName,root->token->token_name,"",st,0,0,0,0);
			if(root->type == -1)
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s\n", "Line ",root->token->line_number
					," : variable <",root->token->token_name,"> is undeclared");
				error2 = 1;
			}
		}
	}
	else
	{
		Ast *lchild = root->child,*rchild = root->child->next;
		checkArithmetic(lchild,st,funName,stdout);
		checkArithmetic(rchild,st,funName,stdout);

		// If type of both child is same and not -1
		// check here of multiplication of same records and division
		if((lchild->type == rchild->type) && (lchild->type != -1))
		{
			if(strcmp(root->data.name,"TK_MUL")== 0)
			{
				if(lchild->type > 1)
				{
					if(print_semantic_error)
						fprintf(stdout, "%s%d%s%s%s%s%s%s%s%s%s\n","Line ",root->token->line_number,
						" : multiplication of records <",
						lchild->token->token_name,"> of type <",variableindex[lchild->type], 
						"> and <",   
						rchild->token->token_name,"> of type <",variableindex[rchild->type]
						,">is not supported");
					error2 = 1;
					root->type = -1;					
				}
				else
					root->type = lchild->type;
			}
			else if(strcmp(root->data.name,"TK_DIV") == 0)
			{
				if(lchild->type > 1)
				{
					if(print_semantic_error)
						fprintf(stdout, "%s%d%s%s%s%s%s%s%s%s%s\n","Line ",root->token->line_number,
						" : division of records <",
						lchild->token->token_name,"> of type <",variableindex[lchild->type], 
						"> and <",   
						rchild->token->token_name,"> of type <",variableindex[rchild->type]
						,">is not supported");
					error2 = 1;
				}
				else
					root->type = lchild->type;
			}
			else
				root->type = lchild->type;

		}
		else if((lchild->type == -1) || (rchild->type == -1))
		{
			root->type = -1;
		}
		else if(((lchild->type == 0) || (lchild->type == 1)) && ((rchild->type == 0) || (rchild->type == 1)))
		{
			// left and right have different types and both are either integer or real.
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s%s%s%s%s%s%s%s%s\n","Line ",lchild->token->line_number,
	" : type mismatch for arithemetic expression type <",variableindex[lchild->type],
	"> of variable <",lchild->token->token_name,"> does not match type <",variableindex[rchild->type],
	"> of variable <",rchild->token->token_name,">" );
			error2 = 1;
		}
		else if((lchild->type == 0) || (lchild->type == 1))
		{
			// right is a record and left is real or float
			if(strcmp(root->data.name,"TK_MUL") == 0)
			{
				root->type = rchild->type;
			}
			else
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s\n","Line ",root->token->line_number," : a variable of type <",
					variableindex[lchild->type],
					"> cannot be divided by a record or added,subtracted to a record");
				error2 = 1;
				root->type = -1;
			}
		}
		else if((rchild->type == 0) || (rchild->type == 1))
		{
			if((strcmp(root->data.name,"TK_MUL") == 0) || (strcmp(root->data.name,"TK_DIV") == 0))
			{
				root->type = lchild->type;
			}
			else
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s\n","Line ",root->token->line_number," : a variable of type <"
						,variableindex[rchild->type],
						"> cannot be added,subtracted to record");
				error2 = 1;
				root->type = -1;
			}				
		}
		else // both are records now check to see if they have same type,as both have different type
		{
			
				root->type = -1;
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s%s%s%s%s%s%s\n","Line ",lchild->token->line_number,
	" : type mismatch for arithemetic expression type <",variableindex[lchild->type],
	"> of variable <",lchild->token->token_name,"> does not match type <",variableindex[rchild->type],
	"> of variable <",rchild->token->token_name,">" );
			error2 = 1;
		}
	}
}
int checkoptype(char *s)
{
	// 0 logicalop
	// 1 relationalop
	// 2 add,mul,div,minus
	if(strcmp(s,"TK_AND") == 0)
	{
		return 0;
	}
	else if(strcmp(s,"TK_OR") == 0)
	{
		return 0;
	}
	else if(strcmp(s,"TK_LT") == 0)
	{
		return 1;
	}
	else if(strcmp(s,"TK_GT") == 0)
	{
		return 1;
	}
	else if(strcmp(s,"TK_GE") == 0)
	{
		return 1;
	}
	else if(strcmp(s,"TK_LE") == 0)
	{
		return 1;
	}
	else if(strcmp(s,"TK_EQ") == 0)
	{
		return 1;
	}
	else if(strcmp(s,"TK_NE") == 0)
	{
		return 1;
	}
	else if(strcmp(s,"TK_PLUS") == 0)
	{
		return 2;
	}
	else if(strcmp(s,"TK_MUL") == 0)
	{
		return 2;
	}
	else if(strcmp(s,"TK_DIV") == 0)
	{
		return 2;
	}
	else if(strcmp(s,"TK_MINUS") == 0)
	{
		return 2;
	}
	else return -1;
}

// optype
// 0 logicalop
// 1 relationalop
// 2 add,mul,div,minus
// Checks a boolean expression
void boolean(Ast *root,symbolTable *st,char *funName,FILE *fp,int iter1,int iter2,int error)
{

	int op = checkoptype(root->data.name);

	if(op == 0) // logicalop
	{
		boolean(root->child,st,funName,stdout,iter1,iter2,error);
		boolean(root->child->next,st,funName,stdout,iter1,iter2,error);
		if(root->child->type == root->child->next->type && root->child->type == 2)
			root->type = 2;
		else
		{
			root->type = -1;
			//fprintf(stdout, "%s\n", "Error:For logical operator types of operand do not match");
		}
	}
	else if(op == 1) // relationalop
	{	
		Ast *lchild,*rchild;
		lchild = root->child;
		rchild = root->child->next;
		while(lchild != NULL)
		{	
			if(strcmp(lchild->data.name,"TK_NUM") == 0)
			{
				lchild->type = 0;
			}
			else if(strcmp(lchild->data.name,"TK_RNUM") == 0)
			{
				install(lchild);	
				lchild->type = 1;
			}
			else if(lchild->child)
			{
				lchild->type = findSymbol(funName,lchild->token->token_name,
					lchild->child->token->token_name,st,0,iter1,iter2,0);	
				if(lchild->type == -1 && error)
				{
					if(print_semantic_error)
						fprintf(stdout, "%s%d%s%s%s%s%s\n", "Line ",lchild->token->line_number
						," : variable <",lchild->token->token_name,".",
						lchild->child->token->token_name,"> is undeclared");
					error2 = 1;
				}	
			}
			else
			{
				lchild->type = findSymbol(funName,lchild->token->token_name,"",st,0,iter1,iter2,0);
				if(lchild->type == -1 && error)
				{
					if(print_semantic_error)
						fprintf(stdout, "%s%d%s%s%s\n", "Line ",lchild->token->line_number
						," : variable <",lchild->token->token_name,"> is undeclared");
					error2 = 1;
				}
			}
			lchild = lchild->next;
		}
		lchild = root->child;
		// only types of both int or both real
		if((lchild->type == rchild->type) && (lchild->type == 0 || lchild->type == 1))
		{
			root->type = 2;
		}
		else if((lchild->type != -1) && (rchild->type != -1) )
		{
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s%s%s%s%s%s%s%s%s%s%s\n","Line ",lchild->token->line_number," : For operator ",
				root->token->token_name," the operands <",lchild->token->token_name,"> has type <"
				,variableindex[lchild->type] ,"> and operand <",rchild->token->token_name,"> has type <",
				variableindex[rchild->type],"> records or type mismatch not supported");
			error2 = 1;
			root->type = -1;
		}
		else // both are records now check to see if they have same type,as both have different type
		{	
			root->type = -1;
		}
	}
	else // tK_not
	{
		boolean(root->child,st,funName,stdout,iter1,iter2,error);
		if(root->child->type == 2)
			root->type = 2;
		else
		{
			root->type = -1;
			//fprintf(stdout, "%s\n", "Error:For not operator types of operand do not match");

		}
	}
}


// checks a assignment operator
void checkassignop(Ast *root,symbolTable *st,char *funName,FILE *fp,int iter)
{
	//printf("sematic %d\n",print_semantic_error );
	if(strcmp(root->token->token_name,"<---") != 0)
		return;
	Ast *lchild = root->child,*rchild = root->child->next;

	// send 1 here for setting used field
	lchild->type = findSymbol(funName,lchild->token->token_name,
		(lchild->child == NULL)? "":lchild->child->token->token_name,st,1,0,0,iter);	
	
	int flag = 0;
	if(lchild->type == -1)
	{
		//printf("sdfsdf");
		if(print_semantic_error)
			fprintf(stdout, "%s%d%s%s%s\n", "Line ",lchild->token->line_number
						," : variable <",lchild->token->token_name,"> is undeclared");
		error2 = 1;
			flag = 1;	
	}
	
	checkArithmetic(rchild,st,funName,stdout);
	
	if((lchild->type != rchild->type)  && ((lchild->type != -1) && (rchild->type != -1))) 
	{
		if(print_semantic_error)
			fprintf(stdout, "%s%d%s%s%s%s%s\n", "Line ",root->token->line_number,
				" : assignment operator has type mismatch,the types are <",variableindex[lchild->type]
				,"> <",variableindex[rchild->type],"> of lhs and rhs");
		error2 = 1;
	}
}


// checks a conditonal statement
void checkconditional(Ast *root,symbolTable *st,char *funName,FILE *fp,int iter)
{

	if(root == NULL || root->child == NULL)
		return;
	root = root->child;
	boolean(root,st,funName,stdout,0,0,1);
	root = root->next; // Dummy Node
	checkStmt(root->child,st,funName,stdout,iter);
	root = root->next; // else part
	root = root->child;
	checkStmt(root,st,funName,stdout,iter);

}

void checkIterative( Ast *root,symbolTable *st,char *funName,FILE *fp)
{
	if(root == NULL)
		return;
	root = root->child; // now pointing to booleanExp
	if(root == NULL)
		return;

	boolean(root,st,funName,stdout,1,0,1);
	
	// root = root->next; // Dummy Node
	// root = root->child; // Stmt
	// assigniter = 0;
	int m = assigniter;
	//printf("Entering while %d\n",m);
	checkStmt(root->next->child,st,funName,stdout,1);
	//printf("Exiting while %d\n",assigniter);
	if(assigniter == m)
	{
		//printf("seic\n");
		if(print_semantic_error)
			fprintf(stdout, "%s%d%s%d%s\n","Line ",root->token->line_number,"-",root->next->next->token->line_number,
				" : None of the variables participating in the iterations of the while loop gets updated"
			);
		error2 = 1;
	}
	// reset the iter fields of variable in boolean expression
	boolean(root,st,funName,stdout,0,1,0);

}

// checks a iostatement
void checkiostmt(Ast *root,symbolTable *st,char *funName,FILE *fp)
{
	if(root == NULL || root->child == NULL)
		return;
	// check if reading a record is supported
	root = root->child;
	if(strcmp(root->data.name,"TK_WRITE") == 0)
	{
		root = root->next;
		if(root)
		{
			root->type = findSymbol(funName,root->token->token_name,
			(root->child == NULL)?"":root->child->token->token_name,st,0,0,0,0);
			if(root->type == -1)
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s\n", "Line ",root->token->line_number
						," : variable <",root->token->token_name,"> is undeclared");
				error2 = 1;
			} 	
		}
	}
	else if(strcmp(root->data.name,"TK_READ") == 0)
	{
		root = root->next;
		if(root)
		{
			root->type = findSymbol(funName,root->token->token_name,
			(root->child == NULL)?"":root->child->token->token_name,st,0,0,0,0);	
			if(strcmp(root->data.name,"TK_NUM") == 0)
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s\n", "Line ",root->token->line_number
						,": cannot read integer <",root->token->token_name,">");
				error2 = 1;
			}
			else if(strcmp(root->data.name,"TK_RNUM") == 0)
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s\n", "Line ",root->token->line_number
						,": cannot read float <",root->token->token_name,">");
				error2 = 1;
			}
			else if(root->type == -1)
			{
				//  add conditions to avoid reading a number
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s\n", "Line ",root->token->line_number
						,": variable <",root->token->token_name,"> is undeclared");
				error2 = 1;
			}
		}

	}
}

// checks a funcall statement
void checkfuncallstmt(Ast *root,symbolTable *st,char *funName,FILE *fp)
{
	if(root == NULL || root->child == NULL)
		return;
	root = root->child;
	int basefunc = getNumAfter(variableTrie,funName),
	calledfunc = getNumAfter(variableTrie,root->next->token->token_name);
	char callfunc[30];
	
	strcpy(callfunc,root->next->token->token_name);

	if(basefunc == calledfunc)
	{
		if(print_semantic_error)
			fprintf(stdout, "%s%d%s\n","Line ",root->token->line_number," : Function recursion not supported");
		error2 = 1;
	}

	functionNode *f  = st->functions;
	int flag = 0;
	while(f && f->name != calledfunc)
	{
		if(f->name == basefunc)
		{
					
			flag = 1;
		}
		f = f->next;
	}
	if(f == NULL)
	{
		// The called function does not exist so return
		if(print_semantic_error)
			fprintf(stdout, "%s%d%s%s%s\n","Line ",root->token->line_number,
			 " : Called Function <",callfunc,
			"()> does not exist");	
		error2 = 1;
		
		return;
	}

	if(flag){
		if(print_semantic_error)
			fprintf(stdout, "%s%d%s%s%s\n","Line ",root->token->line_number,
			 " : Called Function <",callfunc,
				"()> must be above the function from where it is called");
		error2 = 1;
		}
	Ast *output = root->child,*input = root->next->next->child;
	declarationNode *output1 = f->outputParam,*input1 = f->inputParam;
	
	while(output != NULL)
	{
		output->type = findSymbol(funName,output->token->token_name,
			(output->child == NULL)?"":output->child->token->token_name,st,0,0,0,0);
		//printf("Printing output %s %d\n",output->token->token_name,output->type);
		if(output1 == NULL)
		{
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s%s%s\n","Line ",output->token->line_number,
			 " : Called Function <",callfunc,
				"()> does not have these many output parameters");
			error2 = 1;
			break;
		}
		if(output->type == -1)
		{
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s%s%s\n", "Line ",output->token->line_number
						," : variable <",output->token->token_name,"> is undeclared");
			error2 = 1;
		}
		else if(output->type != output1->dataType)
		{
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s%s%s%s%s%s%s%s%s\n","Line ",output->token->line_number, 
					" : Type  <",variableindex[output->type],
					"> of output parameter <",output->token->token_name,"> of function ",callfunc,
					" does not match expected type <",variableindex[output1->dataType],">");
			error2 = 1;
		}
		output = output->next;
		output1 = output1->next;
	}	
	if(output1){
		if(print_semantic_error)
			fprintf(stdout, "%s%d%s%s%s\n","Line ",root->token->line_number,
			 " : Called Function <",callfunc,
				"()> does not have these many parameters");
		error2 = 1;
			}
	while(input != NULL)
	{
		input->type = findSymbol(funName,input->token->token_name,
			(input->child == NULL)?"":input->child->token->token_name,st,0,0,0,0);
		

		if(input1 == NULL)
		{
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s%s%s\n","Line ",input->token->line_number,
			 " : Called Function <",callfunc,
				"()> does not have these many input parameters");
			error2 = 1;
			break;
		}
		if(input->type == -1)
		{
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s%s%s\n", "Line ",input->token->line_number
						," : variable <",input->token->token_name,"> is undeclared");
			error2 = 1;
		}
		else if(input->type != input1->dataType)
		{
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s%s%s%s%s%s%s%s%s\n","Line ",input->token->line_number, 
					" : Type  <",variableindex[input->type],
					"> of input parameter <",input->token->token_name,"> of function ",callfunc,
					" does not match expected type <",variableindex[input1->dataType],">");
			error2 = 1;
		}
		input = input->next;
		input1 = input1->next;
	}	
	if(input1){
		if(print_semantic_error)
			fprintf(stdout, "%s%d%s%s%s\n","Line ",root->token->line_number,
			 " : Called Function <",callfunc,
				"()> does not have these many parameters");
		error2 = 1;
			}
}

// Call different function for different types of statment
void checkStmt(Ast *root,symbolTable *st,char *funName,FILE *fp,int iter)
{
	if(root == NULL)
		return;


	while(root != NULL)
	{
		if(strcmp(root->data.name,"iterativeStmt") == 0)
			checkIterative(root,st,funName,stdout);
		else if(strcmp(root->data.name,"conditionalStmt") == 0)
			checkconditional(root,st,funName,stdout,iter);
		else if(strcmp(root->data.name,"TK_ASSIGNOP")== 0)
			checkassignop(root,st,funName,stdout,iter);
		else if(strcmp(root->data.name,"ioStmt") == 0)
			checkiostmt(root,st,funName,stdout);
		else if(strcmp(root->data.name,"funCallStmt") == 0)
			checkfuncallstmt(root,st,funName,stdout);
		root = root->next;
	}
}

void checkreturnStmt(Ast *root,symbolTable *st,char *funName,FILE *fp)
{
	root = root->child;
	if(root == NULL)
		return;
	if(strcmp(funName,"_main") == 0 && root != NULL)
	{
		if(print_semantic_error)
			fprintf(stdout, "%s%d%s\n","Line :",root->token->line_number,
			 " : Main function can't return value");
		 error2 = 1;
		return;	
	}
	else if(strcmp(funName,"_main"))
	{
		int a = getNumAfter(variableTrie,funName);
		functionNode *f = st->functions;
		while(f && f->name != a)
			f = f->next;
		declarationNode *d;
		if(f)
			d = f->outputParam;
		int z;

		while(root)
		{
			z = findSymbol(funName,root->token->token_name,"",st,0,0,0,0);
			if(z == -1)
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s\n", "Line ",root->token->line_number
						," : variable in return statement <",root->token->token_name,"> is undeclared");
				error2 = 1;
			}
			else if(d == NULL)
			{
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s\n","Line ",root->token->line_number,
						" : The number of variable returned is too large" );
				error2 = 1;
				break;
			}
			else if(z != d->dataType)
			{
				if(print_semantic_error)
					 fprintf(stdout, "%s%d%s%s%s%s%s%s%s%s%s\n","Line ",root->token->line_number," : The type <",
					 	variableindex[z],"> of variable <",root->token->token_name
					 	,"> returned does not match with the type <",
					 	variableindex[d->dataType],"> of the formal output parameter <",
					 	variableindex[d->name],">" );
				 error2 = 1;
			}
			root = root->next;
			d= d->next;
		}
		if(d)
		{
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s\n","Line ",root->token->line_number,
						" : The number of variable returned is too less" );
			error2 = 1;
		}
	}
	// check condition here
}
void checkStmts(Ast *root,symbolTable *st,char *funName,FILE *fp)
{

	root = root->child; // points to stmts
	root = root->next->next;
	checkStmt(root->child,st,funName,stdout,0);
	root = root->next;
	checkreturnStmt(root,st,funName,stdout);
}

void checkused(symbolTable *st,char *funName,FILE *fp)
{
	int a = getNumAfter(variableTrie,funName);
	functionNode *f = st->functions;
	
	while(f && f->name != a)
		f = f->next;

	declarationNode *d;
	d = f->outputParam;
	
	while(d)
	{
		if(d->used != 1)
		{
			if(print_semantic_error)
				fprintf(stdout, "%s%d%s%s%s%s%s\n","Line ",d->line," : Output parameter <",variableindex[d->name],
					"> of function <",funName,"> must be assigned a value" );
			error2 = 1;
		}
		d = d->next;
	}
}

void checkglobalconflict(symbolTable *st,char *funName,FILE *fp)
{

	int a = getNumAfter(variableTrie,funName);
	functionNode *f = st->functions;
	declarationNode *d,*g,*head;
	g = st->globals;

	while(f && f->name != a)
		f = f->next;
	
	d = f->inputParam;

	while(d)
	{
		head = g;
		while(head)
		{
			if(d->name == head->name){
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s%s%s\n","Line ",d->line," : Input parameter <",variableindex[d->name],
						"> of function <",funName,"> has same name as global variable");
					error2 = 1;
					}
			head = head->next;
		}
		d = d->next;
	}

	d = f->outputParam;
	while(d)
	{
		head = g;
		while(head){
			if(d->name == head->name){
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s%s%s\n","Line ",d->line," : Output parameter <",variableindex[d->name],
						"> of function <",funName,"> has same name as global variable");
				error2 = 1;
			}
			head = head->next;
		}
		d = d->next;
	}
	
	d = f->declaration;
	
	while(d)
	{
		head = g;
		while(head){
			if(d->name == head->name){
				if(print_semantic_error)
					fprintf(stdout, "%s%d%s%s%s%s%s\n","Line ",d->line," : variable <",variableindex[d->name],
						"> of function <",funName,"> has same name as global variable");
					error2 = 1;
					}
			head = head->next;
		}
		d = d->next;
	}		
}


void printfloat(FILE *fp)
{
	int i;
	
	for(i = 0;i < floatarrindex;i++)
		fprintf(stdout, "\t%s%s%s%s\n",floatarr[i].name,"  dq  ",floatarr[i].value,",0");
	
}

// main function which performs typechecking 

void checkandExtract(Ast *root,symbolTable *st)
{
	FILE *fp;
	//stdout = fopen("semantic_errors.txt","a");
	floatarrindex = 0;
	if(root == NULL || root->child == NULL)
		return;
	root = root->child;
	while(root)
	{
		
		if(strcmp(root->token->token_name,"_main") != 0)
		{
			checkglobalconflict(st,root->token->token_name,stdout);
			checkStmts(root->child->next->next->next,st,root->token->token_name,stdout);
			checkused(st,root->token->token_name,stdout);
		}
		else
		{
			checkglobalconflict(st,root->token->token_name,stdout);
			checkStmts(root->child->next,st,root->token->token_name,stdout);	
		}
		root = root->next;
	}
	//fclose(stdout);

}
