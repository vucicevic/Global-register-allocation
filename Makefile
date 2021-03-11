PROGRAM	 = GRA
CXX 	 = g++
CXXFLAGS = -std=c++17 -Wall -Wextra



$(PROGRAM): parser.o  lex.yy.o term.o expression.o statement.o functions.o
	$(CXX) $(CXXFLAGS) -o $@ $^  
	
parser.o : parser.tab.cpp parser.tab.hpp statement.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 
	
lex.yy.o : lex.yy.c parser.tab.hpp statement.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 
	
lex.yy.c : lexer.lex 
	flex $<

parser.tab.cpp parser.tab.hpp : parser.ypp
	bison -d -v $<

statement.o : statement.cpp statement.hpp expression.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 
	
expression.o : expression.cpp expression.hpp term.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<  
	
term.o :term.cpp term.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<  

functions.o : functions.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<  

.PHONY : clean

clean :
	rm lex.yy.c *.o *.tab.* *.output $(PROGRAM)
	
