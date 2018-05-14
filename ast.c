// BATCH NUMBER - 57
// Authors:-
// Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
// Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

Ast* getAstNode()
{
  Ast* x = (Ast*)malloc(sizeof(Ast));
  x->child = NULL;
  x->numChild = 0;
  x->parent = NULL;
  x->next = NULL;
  x->prev = NULL;
  bzero(x->data.name,0);
  return x;
}

void copy(Ast *x,ParseTreeNode *y)
{
	strcpy(x->data.name,y->data.name);
	x->data.id = y->data.id;
	x->data.isNonTerminal = y->data.isNonTerminal;
	x->token = malloc(sizeof(TOKEN_INFO));
	x->token->line_number = y->token->line_number;
	x->token->token_id = malloc(sizeof(char)*100);
	x->token->token_name = malloc(sizeof(char)*100);
	strcpy(x->token->token_id,y->token->token_id);
	strcpy(x->token->token_name,y->token->token_name);

}

ParseTreeNode* getptr(treeListNode *head,int pos)
{
	while(pos)
	{

		head = head->next;
		pos--;
	}
	return head->data;
}

void addchild(Ast *x,Ast *y)
{
	if(x)
		x->child = y;
	if(y)
	{
		y->parent = x;
		y->prev = NULL;
	}  
}

void add(Ast *x,Ast *y)
{
	if(x)
		x->next = y;
	if(x && y)
	{
		y->prev = x;
	}
}

// Converts to AST
Ast* convert(ParseTreeNode *root)
{
	Ast *x,*x1,*x2,*x3,*x4,*x5,*xterm;
	x = getAstNode();
	copy(x,root);
	treeListNode *head = root->child->head;
	int z = root->ruleno + 1;
	switch(z)
	{
		case 1:
			//<program> ===> <otherFunctions> <mainFunction> .
			x1 = convert(head->data); // mainFunction
			x2 = convert(head->next->data); // otherFunction
			x3 = x2;
			if(x2 == NULL)
				x3 = x1;
			else{

				while(x2->next != NULL)
				{
					x2 = x2->next;
				}
				
					x2->next = x1;
					x1->prev = x2; 
			}
			
			x->child = x3;
			x3->parent = x;
			break;
		case 2:
			//<mainFunction> ===> TK_MAIN <stmts> TK_END
		  	x1 = convert(head->next->data);
		  	xterm = getAstNode();
		  	copy(xterm,getptr(head,2));
		  	x->child = xterm;
		  	xterm->parent = x;
		  	xterm->next = x1;
		  	x1->prev = xterm; 
			break;
		case 3:
			//<otherFunctions> ===> <function> <otherFunctions>
			
			x = convert(getptr(head,1)); //function
			x2 = convert(head->data);
			x->next = x2;
			if(x2)
				x2->prev = x1;
			break;
		case 4:
			//<otherFunctions> ===> e .
			x = NULL;
			break;
		case 5:
			//<function> ===> TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END 
			xterm = getAstNode();
			copy(xterm,getptr(head,5));
			x1 = convert(getptr(head,4));
			x2 = convert(getptr(head,3));
			x3 = convert(getptr(head,1));
			x->child = xterm;
			xterm->parent = x;
			xterm->next = x1;
			x1->prev = xterm;
			x1->next = x2;
			x2->prev = x1;
			x2->next = x3;
			x3->prev = x2;
			
			break;
		case 6:
			//<input_par> ===> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
			x = convert(head->next->data);
			break;
		case 7:
			//<output_par> ===> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
			x = convert(head->next->data);
			break;
		case 8:
			//<output_par> ===> e .
			x->child = NULL;
			break;
		case 9:
			//<parameter_list> ===> <dataType> TK_ID <remaining_list>
			x1 = convert(getptr(head,2));
			xterm = getAstNode();
			copy(xterm,getptr(head,1));
			x2 =  convert(head->data);
			x1->child = xterm;
			if(xterm)
				xterm->parent = x1;
			//add(x1,x2->child);
			x1->next = x2->child;
			if(x2->child)
				x2->child->prev = x1;
			//addchild(x,x1);
			x->child = x1;
			if(x1)
				x1->parent = x;
			break;
		case 10:
			//<dataType> ===> <primitiveDatatype>
			x = convert(head->data);
			break;
		case 11:
			//<dataType> ===> <constructedDatatype>
			x = convert(head->data);
			break;
		case 12:			
			//<primitiveDatatype> ===> TK_INT
			copy(x,head->data);
			break;
		case 13:
			//<primitiveDatatype> ===> TK_REAL
			copy(x,head->data);			
			break;
		case 14:
			//<constructedDatatype> ===> TK_RECORD TK_RECORDID	
			copy(x,head->data);		
			break;
		case 15:
			//<remaining_list> ===> TK_COMMA <parameter_list>
			x = convert(head->data);				
			break;
		case 16:
			//<remaining_list> ===> e
			x->child = NULL;			
			break;
		case 17:
			//<stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
			// Add check for each being NULL goto all null rules and make their child goto null
			x1 = convert(getptr(head,3));
			x->child = x1;
			x1->parent = x;
			x2 = convert(getptr(head,2)); 
			x3 = convert(getptr(head,1));
			x4 = convert(getptr(head,0));
			x1->next = x2;
			x2->prev = x1;
			x2->next = x3;
			x3->prev = x2;
			x3->next = x4;
			x4->prev = x3;
			break;
		case 18:
			//<typeDefinitions> ===> <typeDefinition> <typeDefinitions>
			x1 = convert(getptr(head,1));
			x->child = x1;
			x1->parent = x;
			x2 = convert(head->data);
			x1->next = x2->child;
			if(x2->child)
				x2->child->prev = x1;
			break;
		case 19:
			//<typeDefinitions> ===> e
			x->child = NULL;			
			break;
		case 20:
			//<typeDefinition> ===> TK_RECORD TK_RECORDID <fieldDefinitions> TK_ENDRECORD TK_SEM
			xterm = getAstNode();
			copy(xterm,getptr(head,3));
			x->child = xterm;
			xterm->parent = x;
			x2 = convert(getptr(head,2));
			xterm->next = x2;
			x2->prev = xterm;
			x2->next = NULL;
			break;
		case 21:
			//<fieldDefinitions> ===> <fieldDefinition> <fieldDefinition> <moreFields>
			x1 = convert(getptr(head,2));
			x2 = convert(getptr(head,1));
			x->child = x1;
			x1->parent = x;
			x1->next = x2;
			x2->prev = x1;
			x3 = convert(getptr(head,0));
			x2->next = x3;
			if(x3)
				x3->prev = x2;
			//add(x2,x3);
			break;
		case 22:
			//<fieldDefinition> ===> TK_TYPE <primitiveDatatype> TK_COLON TK_FIELDID TK_SEM
			xterm = getAstNode();
			copy(xterm,getptr(head,1));
			x1 = convert(getptr(head,3));
			x = x1;
			x1->child = xterm;
			xterm->parent = x1;
			break;
		case 23:
			//<moreFields> ===> <fieldDefinition> <moreFields>	
			x = convert(getptr(head,1));
			x1 = convert(getptr(head,0));
			//add(x,x1);
			x->next = x1;
			if(x1)
				x1->prev = x; 
			break;
		case 24:
			//<moreFields> ===> e
			x = NULL;			
			break;
		case 25:
			//<declarations> ===> <declaration> <declarations>
			x1 = convert(getptr(head,1));
			x2 = convert(head->data);
			//add(x1,x2->child);
			x1->next = x2->child;
			if(x2->child)
				x2->child->prev = x1;
			x->child = x1;
			x1->parent = x;
			break;
		case 26:
			//<declarations> ===> e	
			x->child = NULL;
			break;
		case 27:
			//<declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
			xterm = getAstNode();
			copy(xterm,getptr(head,2));
			x = convert(getptr(head,4));
			x->child = xterm;
			xterm->parent = x;
			x1 = convert(getptr(head,1));
			xterm->child = x1;
			break;
		case 28:
			//<global_or_not> ===> TK_COLON TK_GLOBAL	
			copy(x,getptr(head,0));		
			break;
		case 29:			
			//<global_or_not> ===> e
			x = NULL;
			break;
		case 30:			
			//<otherStmts> ===> <stmt> <otherStmts>
			x1 = convert(getptr(head,1));
			x->child = x1;
			x1->parent = x;
			x2 = convert(head->data);
			x1->next = x2->child;
			if(x2->child)
				x2->child->prev = x1;
			break;
		case 31:
			//<otherStmts> ===> e	
			x->child = NULL;		
			break;
		case 32:			
			//<stmt> ===> <assignmentStmt>
			x = convert(head->data);
			break;
		case 33:
			//<stmt> ===> <iterativeStmt>
			x = convert(head->data);			
			break;
		case 34:
			//<stmt> ===> <conditionalStmt>	
			x = convert(head->data);		
			break;
		case 35:
			//<stmt> ===> <ioStmt>
			x = convert(head->data);		
			break;
		case 36:
			//<stmt> ===> <funCallStmt>
			x = convert(head->data);
			break;
		case 37:
			//<assignmentStmt> ===> <singleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM	
			x1 = convert(getptr(head,3));
			x2 = convert(getptr(head,1));
			copy(x,getptr(head,2));
			x->child = x1;
			x1->parent = x;
			x1->next = x2;
			x2->next = NULL;
			x2->prev = x1;		
			break;
		case 38:
			//<singleOrRecId> ===> TK_ID <new_24>
			copy(x,getptr(head,1));
			x->child = convert(head->data);
			break;
		case 39:
			//<new_24> ===> e
			x = NULL;			
			break;
		case 40:
			//<new_24> ===> TK_DOT TK_FIELDID	
			copy(x,getptr(head,0));	
			break;
		case 41:
			//<funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
			x1 = convert(getptr(head,6));
			x2 = convert(getptr(head,1));
			xterm = getAstNode();
			copy(xterm,getptr(head,4));
			x->child = x1;
			x1->parent = x;
			x1->next = xterm;
			xterm->prev = x1;
			xterm->next = x2;
			x2->prev = xterm;
			x2->next = NULL;
			break;
		case 42:
			//<outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP
			x->child = convert(getptr(head,2));
			break;
		case 43:
			//<outputParameters> ===> e		
			x->child = NULL;
			break;
		case 44:
			//<inputParameters> ===> TK_SQL <idList> TK_SQR
			x1 = convert(getptr(head,1));
			x->child = x1;
			x1->parent = x;						
			break;
		case 45:
			//<iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
			x1 = convert(getptr(head,4));
			x2 = getAstNode(); // Dummy Node
			x5 = getAstNode();
			copy(x5,getptr(head,0));
			strcpy(x2->data.name,"DummyIter");
			x3 = convert(getptr(head,2));
			x4 = convert(getptr(head,1));
			x->child = x1;
			x1->parent = x;
			x1->prev = NULL;
			x1->next = x2;
			x2->prev = x1;
			x2->next = x5;
			x2->child = x3;
			x3->parent = x2;
			x3->prev = NULL;
			x3->next = x4->child;
			if(x4->child)
				x4->child->prev = x3;
			break;
		case 46:			
			//<conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
			x1 = convert(getptr(head,5));
			x2 = getAstNode(); // Dummy Node
			strcpy(x2->data.name,"DummyCond");
			x3 = convert(getptr(head,2));
			x4 = convert(getptr(head,1));
			x5 = convert(getptr(head,0));
			x->child = x1;
			x1->parent = x;
			x1->prev = NULL;
			x1->next = x2;
			x2->prev = x1;
			x2->child = x3;
			x3->parent = x2;
			x3->prev = NULL;
			x3->next = x4->child;
			if(x4->child)
				x4->child->prev = x3;
			x2->next = x5;
			x5->prev = x2;
			x5->next = NULL;			
			break;
		case 47:
			//<elsePart> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
			x1 = convert(getptr(head,2));
			x2 = convert(getptr(head,1));
			x->child = x1;
			x1->parent = x;
			x1->prev = NULL;
			x1->next = x2->child;
			if(x2->child)
				x2->child->prev = x1;
			break;
		case 48:
			//<elsePart> ===> TK_ENDIF
			x->child = NULL;
			break;	
		case 49:
			//<ioStmt> ===> TK_READ TK_OP <singleOrRecId> TK_CL TK_SEM
			xterm = getAstNode();
			copy(xterm,getptr(head,4));
			x1 = convert(getptr(head,2));
			//addchild(x,xterm);
			x->child = xterm;
			if(xterm)
				xterm->parent= x; 
			//add(xterm,x1);
			xterm->next = x1;
			if(x1)
				x1->prev = xterm;
			break;	
		case 50:
			//<ioStmt> ===> TK_WRITE TK_OP <allVar> TK_CL TK_SEM
			xterm = getAstNode();
			copy(xterm,getptr(head,4));
			x1 = convert(getptr(head,2));
			//addchild(x,xterm);
			x->child = xterm;
			if(xterm)
				xterm->parent= x; 
			//add(xterm,x1);
			xterm->next = x1;
			if(x1)
				x1->prev = xterm;			
			break;	
		case 51:
			//<allVar> ===> <var>
			x = convert(head->data);
			break;	
		case 52:
			//<arithmeticExpression> ===> <term> <expPrime>
			x1 = convert(getptr(head,1));
			x2 = convert(getptr(head,0));
			if(x2->child == NULL)
			{
				x = x1;		
			}
			else
			{
				x1->next = x2->child;
				x2->child->prev = x1;
				x2->child = x1;
				x1->parent = x2;
				x1->next->parent = NULL;
				x = x2;
			}
			break;	
		case 53:
			//<expPrime> ===> <lowPrecedenceOperators> <term> <expPrime>
			x1 = convert(getptr(head,0));
			x2 = convert(getptr(head,1));
			x3 = convert(getptr(head,2));
			if(x1->child == NULL)
			{
				x3->child = x2;
				x2->parent = x3;
				x = x3;
			}
			else
			{
				// make x2 first child;
				x1->child->prev = x2;
				x2->next = x1->child;
				x2->next->parent = NULL;
				x2->parent = x1;
				x1->child = x2;
				x3->child = x1;
				x1->parent = x3; 
				x = x3;
			}
			break;	
		case 54:
			//<expPrime> ===> e
			x->child = NULL;
			break;	
		case 55:
			//<term> ===> <factor> <termPrime>
			x1 = convert(getptr(head,1));
			x2 = convert(getptr(head,0));
			if(x2->child == NULL)
				x = x1;			
			else
			{
				x1->next = x2->child;
				x2->child->prev = x1;
				x2->child = x1;
				x1->parent = x2;
				x1->next->parent = NULL;
				x = x2;
			}
			break;	
		case 56:
			//<termPrime> ===> <highPrecedenceOperator> <factor> <termPrime>
			x1 = convert(getptr(head,0));
			x2 = convert(getptr(head,1));
			x3 = convert(getptr(head,2));
			if(x1->child == NULL)
			{
				x3->child = x2;
				x2->parent = x3;
				x = x3;
			}
			else
			{
				x1->child->prev = x2;
				x2->next = x1->child;
				x2->next->parent = NULL;
				x2->parent = x1;
				x1->child = x2;
				x3->child = x1;
				x1->parent = x3; 
				x = x3;
			}			
			break;	
		case 57:
			//<termPrime> ===> e
			x->child = NULL;
			break;	
		case 58:
			//<factor> ===> TK_OP <arithmeticExpression> TK_CL
			x = convert(getptr(head,1));
			break;	
		case 59:
			//<factor> ===> <all>
			x = convert(getptr(head,0));
			break;	
		case 60:
			//<highPrecedenceOperator> ===> TK_MUL
			xterm = getAstNode();
			copy(xterm,getptr(head,0));
			x = xterm;
			break;	
		case 61:
			//<highPrecedenceOperator> ===> TK_DIV
			xterm = getAstNode();
			copy(xterm,getptr(head,0));
			x = xterm;			
			break;	
		case 62:
			//<lowPrecedenceOperators> ===> TK_PLUS .
			xterm = getAstNode();
			copy(xterm,getptr(head,0));
			x = xterm;

			break;	
		case 63:
			//<lowPrecedenceOperators> ===> TK_MINUS .
			xterm = getAstNode();
			copy(xterm,getptr(head,0));
			x = xterm;
			break;	
		case 64:
			//<all> ===> TK_NUM .
			xterm = getAstNode();
			copy(xterm,getptr(head,0));
			x = xterm;
			break;	
		case 65:
			//<all> ===> TK_RNUM .
			xterm = getAstNode();
			copy(xterm,getptr(head,0));
			x = xterm;
			break;	
		case 66:
			//<all> ===> TK_ID <temp> .
			xterm = getAstNode();
			copy(xterm,getptr(head,1));
			x1 = convert(getptr(head,0));
			if(x1->child == NULL)
			{
				x = xterm;
			}
			else
			{
				xterm->child = x1;
				x1->parent = xterm;
				x = xterm;
			}
			break;	
		case 67:
			//<temp> ===> TK_DOT TK_FIELDID
			
			copy(x,getptr(head,0));
			break;	
		case 68:
			//<temp> ===> e .
			x->child = NULL;
			break;
		case 69:
			//<booleanExpression> ===> TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL .
			x2 = convert(getptr(head,1));
			x1 = convert(getptr(head,5));
			x = convert(getptr(head,3));
			x->child = x1;
			x1->parent = x;
			x1->next = x2;
			x2->prev = x1;
			x2->next = NULL;
			break;		
		case 70:
			//<booleanExpression> ===> <var> <relationalOp> <var> .
            x2 = convert(getptr(head,0));
			x1 = convert(getptr(head,2));
			x = convert(getptr(head,1));
			x->child = x1;
			x1->parent = x;
			x1->next = x2;
			x2->prev = x1;
			x2->next = NULL;
			break;		
		case 71:
			//<booleanExpression> ===> TK_NOT TK_OP <booleanExpression> TK_CL .
			copy(x,getptr(head,3));
			x1 = convert(getptr(head,1));
			x->child = x1;
			x1->parent = x;
			break;		
		case 72:
			//<var> ===> TK_ID <new_24> .
			copy(x,getptr(head,1));
			x->child = convert(head->data);
			break;		
		case 73:
			//<var> ===> TK_NUM
			copy(x,getptr(head,0));
			break;		
		case 74:
			//<var> ===> TK_RNUM
			copy(x,getptr(head,0));
			break;		
		case 75:
			//<logicalOp> ===> TK_AND
			copy(x,getptr(head,0));
			break;		
		case 76:
			//<logicalOp> ===> TK_OR
			copy(x,getptr(head,0));
			break;		
		case 77:
			//<relationalOp> ===> TK_LT
			copy(x,getptr(head,0));
			break;		
		case 78:
			//<relationalOp> ===> TK_LE
			copy(x,getptr(head,0));
			break;		
		case 79:
			//<relationalOp> ===> TK_EQ
			copy(x,getptr(head,0));
			break;		
		case 80:
			//<relationalOp> ===> TK_GT
			copy(x,getptr(head,0));
			break;		
		case 81:
			//<relationalOp> ===> TK_GE
			copy(x,getptr(head,0));
			break;		
		case 82:
			//<relationalOp> ===> TK_NE
			copy(x,getptr(head,0));
			break;		
		case 83:
			//<returnStmt> ===> TK_RETURN <optionalReturn> TK_SEM
			x1 = convert(getptr(head,1));
			x->child = x1;
			if(x1)
				x1->parent = x; 
			break;		
		case 84:
			//<optionalReturn> ===> TK_SQL <idList> TK_SQR
			x = convert(getptr(head,1));
			break;		
		case 85:
			//<optionalReturn> ===> e
			x = NULL;
			break;		
		case 86:
			//<idList> ===> TK_ID <more_ids>
			xterm = getAstNode();
			copy(xterm,getptr(head,1));
			x1 = convert(getptr(head,0));
			x = xterm;
			xterm->next = x1;
			if(x1)
				x1->prev = xterm; 
			break;		
		case 87:
			//<more_ids> ===> TK_COMMA <idList>
			x = convert(getptr(head,0));
			break;		
		case 88:
			//<more_ids> ===> e
			x = NULL;
			break;	
		default:
        	printf("Hallo\n");
        	break;		
	}
	
	return x;

}

int printAst(Ast *x,char *s,int flag)
{
	int nodes;
	if(x == NULL)
		return 0;

	// printf( "%-25s %-5d %-15s %-7s %-25s %-7s %-15s\n"
 //      , (root->data.isNonTerminal? "---":root->token->token_name)
 //      , root->token->line_number
 //      , root->token->token_id
 //      , (!root->data.isNonTerminal && check_number(root)? root->token->token_name:"---")
 //      , (root->parent? root->parent->data.name:"ROOT")
 //      , (!root->data.isNonTerminal ? "yes":"no")
 //      , (root->data.isNonTerminal? root->data.name:"---"));

if(flag){
	if(x->token){
		printf( "%-25s %-5d %-15s %-25s %-7s %-15s\n"
    	 , (x->data.isNonTerminal? "---":x->token->token_name)
      	, x->token->line_number
      	, x->token->token_id
      	, s
      	, (!x->data.isNonTerminal ? "yes":"no")
	     , (x->data.isNonTerminal? x->data.name:"---"));
	}
	else
	{
		printf( "%-25s %-5s %-15s %-25s %-7s %-15s\n"
    	 , "---"
      	, "---"
      	, "---"
      	, s
      	, (!x->data.isNonTerminal ? "yes":"no")
	     , (x->data.isNonTerminal? x->data.name:"---"));	
	}

}
	nodes = 1;
	Ast *head = x->child;
	while(head != NULL)
	{
		nodes += printAst(head,x->data.name,flag);
		head = head->next;
	}
	return nodes;
}