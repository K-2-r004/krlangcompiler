#pragma once
#include "lexer.h"
#include "ast.h"
#include <memory>
#include "symbol_table.h"
class Parser {

Lexer lexer;
Token current;
SymbolTable symbols;
void eat(TokenType type);

std::unique_ptr<ASTNode> expression();
std::unique_ptr<ASTNode> term();
std::unique_ptr<ASTNode> factor();

std::unique_ptr<ASTNode> statement();
std::unique_ptr<ASTNode> condition();

public:

Parser(Lexer l);

std::unique_ptr<ProgramNode> parse();

};