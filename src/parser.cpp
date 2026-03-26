#include "parser.h"
#include <stdexcept>

Parser::Parser(Lexer l)
: lexer(l){

current = lexer.nextToken();

}

void Parser::eat(TokenType type){

if(current.type == type)
current = lexer.nextToken();

else{

throw std::runtime_error(
"Syntax Error at line " +
std::to_string(current.line)
);

}

}

std::unique_ptr<ASTNode> Parser::factor(){

Token tok = current;

if(tok.type == TokenType::NUMBER){

eat(TokenType::NUMBER);

return std::make_unique<NumberNode>(stoi(tok.value));

}

if(tok.type == TokenType::IDENTIFIER){

symbols.check(tok.value);

eat(TokenType::IDENTIFIER);

return std::make_unique<VariableNode>(tok.value);

}

throw std::runtime_error("Invalid factor");

}

std::unique_ptr<ASTNode> Parser::term(){

auto node = factor();

while(current.type == TokenType::MUL ||
current.type == TokenType::DIV){

std::string op = current.value;

if(current.type == TokenType::MUL)
eat(TokenType::MUL);
else
eat(TokenType::DIV);

node = std::make_unique<BinaryNode>(
op,
std::move(node),
factor()
);

}

return node;

}

std::unique_ptr<ASTNode> Parser::expression(){

auto node = term();

while(current.type == TokenType::PLUS ||
current.type == TokenType::MINUS){

std::string op = current.value;

if(current.type == TokenType::PLUS)
eat(TokenType::PLUS);
else
eat(TokenType::MINUS);

node = std::make_unique<BinaryNode>(
op,
std::move(node),
term()
);

}

return node;

}

std::unique_ptr<ASTNode> Parser::statement(){

if(current.type == TokenType::VAR){

eat(TokenType::VAR);

std::string name = current.value;

eat(TokenType::IDENTIFIER);

symbols.declare(name);

eat(TokenType::ASSIGN);

auto expr = expression();

return std::make_unique<VarDeclNode>(
name,
std::move(expr)
);

}

if(current.type == TokenType::PRINT){

eat(TokenType::PRINT);

auto expr = expression();

return std::make_unique<PrintNode>(
std::move(expr)
);

}

throw std::runtime_error("Invalid statement");

}

std::unique_ptr<ProgramNode> Parser::parse(){

auto program = std::make_unique<ProgramNode>();

eat(TokenType::START);

while(current.type != TokenType::END){

program->add(statement());

}

eat(TokenType::END);

return program;

}