OBJS = src/Symboltable.o src/SyntaxTree.o src/CodeGeneration.o src/Queue.o src/Stack.o

compiler: lex yacc Symboltable SyntaxTree Queue Stack CodeGeneration
	gcc -g src/lex.yy.c src/y.tab.c $(OBJS) -o bin/compiler

lex: src/compiler.l yacc
	lex src/compiler.l
	mv lex.yy.c src/	

yacc: src/compiler.y
	yacc -d src/compiler.y
	mv y.tab.c y.tab.h src/

Symboltable: src/Symboltable.c include/Symboltable.h
	gcc -Iinclude -c src/Symboltable.c -o src/Symboltable.o

SyntaxTree: src/SyntaxTree.c include/SyntaxTree.h
	gcc -Iinclude -c src/SyntaxTree.c -o src/SyntaxTree.o

Queue: src/Queue.c include/Queue.h
	gcc -Iinclude -c src/Queue.c -o src/Queue.o

Stack: src/Stack.c include/Stack.h
	gcc -Iinclude -c src/Stack.c -o src/Stack.o

CodeGeneration: src/CodeGeneration.c include/CodeGeneration.h
	gcc -Iinclude -c src/CodeGeneration.c -o src/CodeGeneration.o


assembly:
	mips-linux-gnu-gcc-10 file.c -S -o file.s
	mips-linux-gnu-gcc-10  file.s -o file.out
	qemu-mips -L /usr/mips-linux-gnu/ file.out

clean_assembly:
	rm -f file.out file.s code.out

clean:
	rm -f src/y.tab.h src/lex.yy.c src/y.tab.c src/*.o
	rm -f bin/compiler

test:
	mips-linux-gnu-gcc-10	code.s -o code.out
	qemu-mips -L /usr/mips-linux-gnu/ code.out