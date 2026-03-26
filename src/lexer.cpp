#include "lexer.h"
#include <cctype>

Lexer::Lexer(std::string src)
: source(src), pos(0), line(1) {}

char Lexer::peek(){
if(pos >= source.size()) return '\0';
return source[pos];
}

char Lexer::advance(){

char c = source[pos++];

if(c == '\n')
line++;

return c;
}

void Lexer::skipWhitespace(){

while(isspace(peek()))
advance();

}

Token Lexer::identifier(){

std::string value;

while(isalnum(peek()))
value += advance();

if(value=="start") return {TokenType::START,value,line};
if(value=="end") return {TokenType::END,value,line};
if(value=="var") return {TokenType::VAR,value,line};
if(value=="print") return {TokenType::PRINT,value,line};

return {TokenType::IDENTIFIER,value,line};
}

Token Lexer::number(){

std::string value;

while(isdigit(peek()))
value += advance();

return {TokenType::NUMBER,value,line};
}

Token Lexer::nextToken(){

skipWhitespace();

char c = peek();

if(c=='\0')
return {TokenType::END_OF_FILE,"",line};

if(isalpha(c))
return identifier();

if(isdigit(c))
return number();

advance();

switch(c){

case '+': return {TokenType::PLUS,"+",line};
case '-': return {TokenType::MINUS,"-",line};
case '*': return {TokenType::MUL,"*",line};
case '/': return {TokenType::DIV,"/",line};
case '=': return {TokenType::ASSIGN,"=",line};

}

return {TokenType::END_OF_FILE,"",line};
}