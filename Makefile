GRA: parser.o  lex.yy.o tip.o izraz.o naredba.o	funkcije.o
	g++ -std=c++14 -o $@ $^ -Wall -Wextra 
	
parser.o : parser.tab.cpp 
	g++ -std=c++14 -c -o $@ $< -Wall -Wextra
	
lex.yy.o : lex.yy.c 
	g++ -std=c++14 -c -o $@ $< -Wall -Wextra
	
lex.yy.c : lexer.lex 
	flex $<

parser.tab.cpp parser.tab.hpp : parser.ypp
	bison -d -v $<

naredba.o : naredba.cpp naredba.hpp
	g++ -std=c++14 -c -o $@ $< -Wall -Wextra -Wno-sign-compare
	
izraz.o : izraz.cpp izraz.hpp
	g++ -std=c++14 -c -o $@ $< -Wall -Wextra
	
tip.o :	tip.cpp tip.hpp
	g++ -std=c++14 -c -o $@ $< -Wall -Wextra

funkcije.o : funkcije.cpp
	g++ -std=c++14 -c -o $@ $< -Wall -Wextra

.PHONY : clean

clean :
	rm lex.yy.c *.o *.tab.* *.output GRA
	
