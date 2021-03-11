%option nounput
%option noyywrap

%{

    #include <iostream>
    #include <string>
    #include <cstdlib>
    
    using namespace std;

    
    #include "statement.hpp"
    #include "parser.tab.hpp"
%}

%%

">="            { yylval.s = new string(yytext); return relop_token;    }
"<="            { yylval.s = new string(yytext); return relop_token;    }
">"             { yylval.s = new string(yytext); return relop_token;    }
"<"             { yylval.s = new string(yytext); return relop_token;    }

":="            { return assign_token;                                  }

"ifFalse"       { return if_token;                                      }

"goto"          { return goto_token;                                    }
        
"return"        { return return_token;                                  }

[a-z]           { yylval.s = new string(yytext); return id_token;       }

[t][1-9][0-9]*  { yylval.s = new string(yytext); return id_token;       }
                                
[a-z][a-z]+     { yylval.s = new string(yytext); return function_token; }
                                
(-)?[0-9]+      { yylval.i = atoi(yytext); return num_token;            }
                                
[-+*/()\n]      { return *yytext; }   

[ \t]           {   }

.               { cerr << "Lex error. Unknown char : " << *yytext <<endl; exit(EXIT_FAILURE); }

%%

