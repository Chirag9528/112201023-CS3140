cc = gcc
LEX = lex
YACC = yacc
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin
CFLAGS = -Iinclude

#object files
OBJS = $(SRC_DIR)/Symboltable.o ${SRC_DIR}/SyntaxTree.o ${SRC_DIR}/Queue.o

Compiler: $(SRC_DIR) $(BIN_DIR) $(OBJS) lex yacc
	@$(CC) -g $(SRC_DIR)/lex.yy.c $(SRC_DIR)/y.tab.c $(OBJS) -o $(BIN_DIR)/Compiler

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



# Clean target
clean:
	@rm -f $(SRC_DIR)/y.tab.h $(SRC_DIR)/lex.yy.c $(SRC_DIR)/y.tab.c $(SRC_DIR)/*.o
	@rm -f $(BIN_DIR)/Compiler