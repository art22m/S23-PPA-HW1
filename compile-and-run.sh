bison -d -t parser.y ; flex lexer.l ; gcc lex.yy.c parser.tab.c ast.c ; cat prog | ./a.out ; g++ -std=c++17 checker.cpp -o checker.out ; ./checker.out
