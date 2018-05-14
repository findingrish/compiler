# BATCH NUMBER - 57
# Authors:-
# Rishabh Singh  2012B4A7691P  f2012691@pilani.bits-pilani.ac.in
# Anchit Jain    2012B3A7570P  f2012570@pilani.bits-pilani.ac.in
# nasm -felf64 first.asm && gcc first.o && ./a.out
all: toycompiler

toycompiler: driver.o lexer.o parser.o ast.o symbolTable.o typeCheck_and_semanticAnalysis.o codeGen.o
	gcc -pg -g -fprofile-arcs driver.o lexer.o parser.o ast.o symbolTable.o typeCheck_and_semanticAnalysis.o codeGen.o -o toycompiler


lexer.o: lexer.c
	gcc -pg -g -fprofile-arcs -c lexer.c
parser.o: parser.c
	gcc -pg -g -fprofile-arcs -c parser.c	
ast.o: ast.c
	gcc -pg -g -fprofile-arcs -c ast.c
symbolTable.o: symbolTable.c
	gcc -pg -g -fprofile-arcs -c symbolTable.c
typeCheck_and_semanticAnalysis.o: typeCheck_and_semanticAnalysis.c
	gcc -pg -g -fprofile-arcs -c typeCheck_and_semanticAnalysis.c
codeGen.o: codeGen.c
	gcc -pg -g -fprofile-arcs -c codeGen.c	
driver.o: driver.c
	gcc -pg -g -fprofile-arcs -c driver.c

clean:
	rm -f *.o
	rm -f toycompiler *.gcda gmon.out    
	rm -f FirstSet.txt FollowSet.txt  comments.txt 
	rm -f syntax_errors.txt sematic_errors.txt
	rm -f a.out *.asm
