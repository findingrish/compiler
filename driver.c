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

void comments_free_code(FILE *fp)
{

  char buff[600];
  char *pos;
  int ind;

  while (fgets(buff, sizeof(buff), fp)!=NULL)
  {
    if ((pos = strchr(buff, '%'))!=NULL)
    {
      ind = (int) (pos - buff);
      if(ind != 0)
        buff[ind-1] = '\n';
      bzero(pos, sizeof(pos));
    }
    printf("%s", buff);
    bzero(buff, sizeof(buff));
  }
}

int main(int argc, char *argv[])
{

  if (argc < 3 || argc >= 4)
  {
    fprintf(stderr, "Usage: %s InputFile OutputFile\n", argv[0]);
    exit(-1);
  }

  printf("\n\n");
  printf("LEVEL #4 : Symbol Table,Typechecking,Semantic Analysis,Code Generation modules work.\n");
  printf("\n\n");

  char *inputFile = argv[1], *outputFile = argv[2];
  char option_entered[100];
  int option_val;

  TOKEN_INFO *t1;
  
  populateHash();
  Gpair g = populateRules();

  FindFirstSet(g.reducedGrammar);
 
  FindFollowSet(g.reducedGrammar);
 
  setSynch(g.reducedGrammar);
 
  printFirstSet();
  printFollowSet();

  int **parsetable = getParseTable();
  FirstSetNode *t;
  makeparsetable(parsetable, Firstset, FollowSet, g.totalGrammar);
  FILE *fp, *fp_error, *fp_comment, *fp_tree,*fp_ast,*fp_sem_error;
  Ast *astRoot;
  declarationNode *d;
  int l,syntax_error,semantic_error;
  symbolTable *st;
  char *rt;
  long size1,size2;
  int treenode,astnode;
  int treenodesize,astnodesize;
  treenodesize = sizeof(ParseTreeNode);
  
  astnodesize = treenodesize;
  float percentage;
  while (1)
  {

    bzero(option_entered, sizeof(option_entered));
    printf("Enter desired Option value\n"
               "1 - Print token list\n"
               "2 - Parse input source code and Produce Parse Tree on console\n"
               "3 - Generate ast and print on console\n"
               "4 - Print amount of allocated memory to Parse Tree and Ast\n"
               "5 - Print Symbol Table\n"
               "6 - Compile Front End to verify syntactic and Sematic Correctness\n"
               "7 - Produce Assembly Code\n"
               );
    scanf("%s", option_entered);

    option_val = atoi(option_entered);

    if (option_val < 1 || option_val >= 8)
    {
      printf("Invalid option entered.\n");
      continue;
    }

    switch (option_val)
    {
    
      case 1 : // Generate Token List
        error1 = 0;
        error2 = 0;
        print_syntax_error = 0;
        print_semantic_error = 0;

         fp = fopen(inputFile, "r");
        // fp_error = fopen("syntax_errors.txt", "w");
        // fp_comment = fopen("comments.txt", "w");

        getNextToken(fp, fp_error, fp_comment, 1);
        t1 = getNextToken(fp, fp_error, fp_comment, 0);
        while (strcmp(t1->token_name, "$")!=0)
        {
          printf("%d %s %s\n", t1->line_number, t1->token_name, t1->token_id);
          t1 = getNextToken(fp, fp_error, fp_comment, 0);
        }

        fclose(fp);
        // fclose(fp_error);
        // fclose(fp_comment);
        break;
      case 2 : // Parse Source Code and Print Parse Tree On Console
        error1 = 0;
        error2 = 0;
        print_syntax_error = 0;
        print_semantic_error = 0;
        fp = fopen(inputFile, "r");
        // fp_error = fopen("syntax_errors.txt", "w");
        // fp_comment = fopen("comments.txt", "w");

        getNextToken(fp, fp_error, fp_comment, 1);
        stack_simulate(fp, fp_error, fp_comment, parsetable, g.totalGrammar);

        fclose(fp);
        // fclose(fp_error);
        // fclose(fp_comment);
        //fp_tree = fopen(, "w");

        // getNextToken(fp, fp_error, fp_comment, 0);
        printf("%-25s %-5s %-15s %-7s %-25s %-7s %-15s\n",
          "Lex_Cur_Node","Line","Token","ValIfNum","Parent","IsLeaf","NodeSymbol");
        printDFSTree(rootNode,1);
        if(error1 == 1)
          printf("%s\n","Lexical errors were in this testcase,\nPress 6 to see the errors\n" );
        //fclose(fp_tree);
        break;
      
      case 3: // Generate ast and print on console
      // step 2
        error1 = 0;
        error2 = 0;
        print_syntax_error = 0;
        print_semantic_error = 0;

        fp = fopen(inputFile, "r");
        // fp_error = fopen("syntax_errors.txt", "w");
        // fp_comment = fopen("comments.txt", "w");

        getNextToken(fp, fp_error, fp_comment, 1);
        stack_simulate(fp, fp_error, fp_comment, parsetable, g.totalGrammar);

        fclose(fp);
        // fclose(fp_error);
        // fclose(fp_comment);
        
      // step2 close

          // fp_error = fopen("syntax_errors.txt", "r");
          // fseek(stdout,0,SEEK_END);
          // size1 = ftell(fp_error);
          // fclose(fp_error);
         // printf("%ld\n",size1 );
          if(error1 > 0)
          {
            printf("%s\n","Lexical error can't build Ast\nPress 6 to see errors" );
            syntax_error = 1;
          }
          else
          {
              printf("Building Ast\n");
              syntax_error = 0;
              astRoot = convert(rootNode);
              printf("%-25s %-5s %-15s %-7s %-25s %-7s %-15s\n",
                "Lex_Cur_Node","Line","Token","ValIfNum","Parent","IsLeaf","NodeSymbol"); 
              // Print Ast On console
              printAst(astRoot,"root",1); 
          }
          break;
        case 4: // Print memory
        // Repeat step 2 & 3
        // step 2
        error1 = 0;
        error2 = 0;
        print_syntax_error = 0;
        print_semantic_error = 0;

        fp = fopen(inputFile, "r");
        // fp_error = fopen("syntax_errors.txt", "w");
        // fp_comment = fopen("comments.txt", "w");

        getNextToken(fp, fp_error, fp_comment, 1);
        stack_simulate(fp, fp_error, fp_comment, parsetable, g.totalGrammar);

        fclose(fp);
        // fclose(fp_error);
        // fclose(fp_comment);
        // getNextToken(fp, fp_error, fp_comment, 1);
        treenode = printDFSTree(rootNode,0);        
      
        // step2 close

         // fp_error = fopen("syntax_errors.txt", "r");
          // fseek(stdout,0,SEEK_END);
          // size1 = ftell(fp_error);
         // fclose(fp_error);
         // printf("%ld\n",size1 );
          if(error1 > 0)
          {
            printf("%s\n","Lexical error can't build Ast,So can't compare\nPress 6 to see errors" );

            syntax_error = 1;
          }
          else
          {
              //printf("Building Ast\n");
              syntax_error = 0;
              astRoot = convert(rootNode);
              //printf("%-25s %-5s %-15s %-7s %-25s %-7s %-15s\n",
                //"Lex_Cur_Node","Line","Token","ValIfNum","Parent","IsLeaf","NodeSymbol"); 
              // Print Ast On console
              astnode = printAst(astRoot,"root",0);
              printf("%s%d%s%d\n","Parse Tree Number of Node = ",treenode," Allocated Memory = ",
                treenode*treenodesize);
              printf("%s%d%s%d\n","Parse Tree Number of Node = ",astnode," Allocated Memory = ",
                astnode*astnodesize);
              percentage = (treenode*treenodesize - astnode*astnodesize)*1.0;
              percentage = percentage/(treenode*treenodesize); 
              printf("%s%f%s\n","Compression Percentage = ", percentage*100, "%");
              printf("%s%f%s\n","Ast requires ",100-percentage*100,"% of space in comparison to Parse Tree\n" );
          }

          break; 

        case 5:// Print Symbol Table
        // repeating steps 2-4
        error1 = 0;
        error2 = 0;
        print_syntax_error = 0;
        print_semantic_error = 0;

        fp = fopen(inputFile, "r");
        // fp_error = fopen("syntax_errors.txt", "w");
        // fp_comment = fopen("comments.txt", "w");

        getNextToken(fp, fp_error, fp_comment, 1);
        stack_simulate(fp, fp_error, fp_comment, parsetable, g.totalGrammar);

        fclose(fp);
        // fclose(fp_error);
        // fclose(fp_comment);
           
      
        // step2 close

        //  fp_error = fopen("syntax_errors.txt", "r");
          // fseek(stdout,0,SEEK_END);
          // //size1 = ftell(fp_error);
          // fclose(fp_error);
         // printf("%ld\n",size1 );
          if(error1 > 0)
          {
            printf("%s\n","Lexical error can't build Ast,Symbol Table\nPress 6 to see errors" );

            syntax_error = 1;
          }
          else
          {
              syntax_error = 0;
              astRoot = convert(rootNode);     
              // Build Symbol Table and Print
              st = populateSymbolTable(astRoot,1);
          }
        break;                
        case 6: //Compile Front End to verify syntactic and Sematic Correctness
        // Repeating steps 2-5
        error1 = 0;
        error2 = 0;
        print_syntax_error = 1;
        print_semantic_error = 1;

        fp = fopen(inputFile, "r");
        // fp_error = fopen("syntax_errors.txt", "w");
        // fp_comment = fopen("comments.txt", "w");

        getNextToken(fp, fp_error, fp_comment, 1);
        stack_simulate(fp, fp_error, fp_comment, parsetable, g.totalGrammar);

          fclose(fp);
          // fclose(fp_error);
          // fclose(fp_comment);
           
      
        // step2 close

     //     fp_error = fopen("syntax_errors.txt", "r");
       //    fseek(stdout,0,SEEK_END);
       // //   size1 = ftell(fp_error);
       //    fclose(fp_error);
         // printf("%ld\n",size1 );
          if(error1 > 0)
          {
            printf("%s\n","Lexical errors are listed above" );
            printf("%s\n","Lexical error can't build Ast,Symbol Table" );

            syntax_error = 1;
          }
          else
          {
              syntax_error = 0;
              astRoot = convert(rootNode);
              // Build Symbol Table and Print
              st = populateSymbolTable(astRoot,0);
              checkandExtract(astRoot,st);     
          }               
        // repeating steps
        break;
        case 7: // Produce Assembly Code
        // Repeating 2-6 
        error1 = 0;
        error2 = 0;
        print_syntax_error = 0;
        print_semantic_error = 0;

        fp = fopen(inputFile, "r");
        // fp_error = fopen("syntax_errors.txt", "w");
        // fp_comment = fopen("comments.txt", "w");

        getNextToken(fp, fp_error, fp_comment, 1);
        stack_simulate(fp, fp_error, fp_comment, parsetable, g.totalGrammar);
        fclose(fp);
        // fclose(fp_error);
        // fclose(fp_comment);
        // // step2 close
        //fp_error = fopen("syntax_errors.txt", "r");
        // fseek(stdout,0,SEEK_END);
        // size1 = ftell(fp_error);
        //fclose(fp_error);
         // printf("%ld\n",size1 );
        if(error1 > 0)
        {
          printf("%s\n","Lexical error can't build Ast,Symbol Table,Generate Code\nPress 6 to see errors" );
          syntax_error = 1;
        }
        else
        {
            syntax_error = 0;
            astRoot = convert(rootNode);
            // Build Symbol Table and Print
            st = populateSymbolTable(astRoot,0);
            checkandExtract(astRoot,st);
           // fp_sem_error = fopen("semantic_errors.txt","r");
            // fseek(stdout,0,SEEK_END);
            // size2 = ftell(fp_sem_error);
           // fclose(fp_sem_error);
            //printf("%ld\n",size2);
            if(error2 > 0)
              printf("%s\n","Semantic Error can't generate code\nPress 6 to errors" );
            else
            {  
                printf("Compiled Successfully!!\n"); 
                code(astRoot,st,outputFile);
            }        
        }                 
        break;  
      default:
        printf("Invalid option entered.\n");
        break;
    }
    printf("\n\n");

  }
}