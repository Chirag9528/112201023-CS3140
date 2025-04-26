cc = gcc
LEX = lex
YACC = yacc
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin
CFLAGS = -Iinclude

#object files
OBJS = $(SRC_DIR)/Symboltable.o ${SRC_DIR}/SyntaxTree.o ${SRC_DIR}/CodeGeneration.o ${SRC_DIR}/Queue.o ${SRC_DIR}/Stack.o

compiler: $(SRC_DIR) $(BIN_DIR) $(OBJS) lex yacc
	@$(CC) -g $(SRC_DIR)/lex.yy.c $(SRC_DIR)/y.tab.c $(OBJS) -o $(BIN_DIR)/compiler

#lex and yacc
lex: $(SRC_DIR)/compiler.l yacc
	@$(LEX) $(SRC_DIR)/compiler.l
	@mv lex.yy.c $(SRC_DIR)/	

yacc: $(SRC_DIR)/compiler.y
	@$(YACC) -Wcounterexamples -d $(SRC_DIR)/compiler.y
	@mv y.tab.c y.tab.h $(SRC_DIR)/

# Object file compilation
$(SRC_DIR)/Symboltable.o: $(SRC_DIR)/Symboltable.c $(INCLUDE_DIR)/Symboltable.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)/Symboltable.c -o $(SRC_DIR)/Symboltable.o

$(SRC_DIR)/SyntaxTree.o: $(SRC_DIR)/SyntaxTree.c $(INCLUDE_DIR)/SyntaxTree.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)/SyntaxTree.c -o $(SRC_DIR)/SyntaxTree.o

$(SRC_DIR)/Queue.o: $(SRC_DIR)/Queue.c $(INCLUDE_DIR)/Queue.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)/Queue.c -o $(SRC_DIR)/Queue.o

$(SRC_DIR)/Stack.o: $(SRC_DIR)/Stack.c $(INCLUDE_DIR)/Stack.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)/Stack.c -o $(SRC_DIR)/Stack.o

$(SRC_DIR)/CodeGeneration.o: $(SRC_DIR)/CodeGeneration.c $(INCLUDE_DIR)/CodeGeneration.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)/CodeGeneration.c -o $(SRC_DIR)/CodeGeneration.o

# Clean target
clean:
	@rm -f $(SRC_DIR)/y.tab.h $(SRC_DIR)/lex.yy.c $(SRC_DIR)/y.tab.c $(SRC_DIR)/*.o
	@rm -f $(BIN_DIR)/compiler


assembly:
	mips-linux-gnu-gcc-10 file.c -S -o file.s
	mips-linux-gnu-gcc-10  file.s -o file.out
	qemu-mips -L /usr/mips-linux-gnu/ file.out

clean_assembly:
	rm -f file.out file.s code.out

test:
	mips-linux-gnu-gcc-10	code.s -o code.out
	qemu-mips -L /usr/mips-linux-gnu/ code.out