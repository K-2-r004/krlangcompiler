#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer.h"
#include "parser.h"

std::string tokenName(TokenType type){

switch(type){

case TokenType::START: return "START";
case TokenType::END: return "END";
case TokenType::VAR: return "VAR";
case TokenType::PRINT: return "PRINT";

case TokenType::IDENTIFIER: return "IDENTIFIER";
case TokenType::NUMBER: return "NUMBER";

case TokenType::PLUS: return "PLUS";
case TokenType::MINUS: return "MINUS";
case TokenType::MUL: return "MUL";
case TokenType::DIV: return "DIV";

case TokenType::ASSIGN: return "ASSIGN";

default: return "EOF";

}

}


int main(){


std::ifstream file("examples/test.kr");

std::stringstream buffer;
buffer << file.rdbuf();


std::string source = buffer.str();

Lexer lexer(source);

Token tok = lexer.nextToken();

std::cout << "TOKENS\n";

while(tok.type != TokenType::END_OF_FILE){

std::cout
<< tokenName(tok.type)
<< " : "
<< tok.value
<< " (line "
<< tok.line
<< ")\n";

tok = lexer.nextToken();

}
Parser parser{Lexer(source)};

auto program = parser.parse();

program->print(0);

}