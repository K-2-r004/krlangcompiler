#pragma once
#include "token.h"
#include <string>

class Lexer {

std::string source;
size_t pos;
int line;

char peek();
char advance();
void skipWhitespace();

Token identifier();
Token number();

public:

Lexer(std::string src);

Token nextToken();

};