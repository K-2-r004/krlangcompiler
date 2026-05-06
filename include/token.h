#pragma once
#include <string>

enum class TokenType {

START, END,

VAR,
PRINT,

IDENTIFIER,
NUMBER,

PLUS, MINUS,
MUL, DIV,

IF, ENDIF,
GREATER, LESS,

WHILE,ENDWHILE,

ASSIGN,

END_OF_FILE
};

struct Token {

TokenType type;
std::string value;
int line;

Token()
: type(TokenType::END_OF_FILE), value(""), line(0) {}

Token(TokenType t, std::string v, int l)
: type(t), value(v), line(l) {}

};