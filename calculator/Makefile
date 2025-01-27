run:
	lex calculator.l
	yacc -d calculator.y
	gcc y.tab.c lex.yy.c -o calculator -ll 
	./calculator

clean:
	rm lex.yy.c y.tab.c y.tab.h calculator
