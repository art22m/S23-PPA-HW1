#!/bin/bash

# Given
TEST='1 a := 2;\n1 b := 3'
printf "$TEST" > test.in.txt

# When
output=$(bison -d -t parser.y ; flex lexer.l ; gcc lex.yy.c parser.tab.c ast.c -o test.ast.out ; cat test.in.txt | ./test.ast.out ; g++ -std=c++17 checker.cpp -o test.checker.out ; ./test.checker.out)

# Then
if echo "$output" | grep -q "Lines 1 2 have the same number"; then
    echo "Pass"
else
    echo "Fail"
fi