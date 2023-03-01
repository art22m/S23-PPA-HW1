#!/bin/bash

# Given
TEST="1 b := b"
printf "%s" "$TEST" > test.in.txt

# When
output=$(bison -d -t parser.y ; flex lexer.l ; gcc lex.yy.c parser.tab.c ast.c -o test.ast.out ; cat test.in.txt | ./test.ast.out ; g++ -std=c++17 checker.cpp -o test.checker.out ; ./test.checker.out)

# Then
if echo "$output" | grep -q "ERR: Recursive assignment"; then
    echo "Pass"
else
    echo "Fail"
fi