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

    // VAR declaration
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

    // 🔥 NEW: Assignment support
    if(current.type == TokenType::IDENTIFIER){

        std::string name = current.value;

        symbols.check(name);   // ensure variable exists

        eat(TokenType::IDENTIFIER);

        eat(TokenType::ASSIGN);

        auto expr = expression();

        return std::make_unique<AssignmentNode>(name, std::move(expr));
    }

    // PRINT
    if(current.type == TokenType::PRINT){

        eat(TokenType::PRINT);

        auto expr = expression();

        return std::make_unique<PrintNode>(
            std::move(expr)
        );
    }

    // IF LOOP
    if(current.type == TokenType::IF){

        eat(TokenType::IF);

        auto cond = condition();

        auto node = std::make_unique<IfNode>(std::move(cond));

        while(current.type != TokenType::ENDIF){

            node->add(statement());
        }

        eat(TokenType::ENDIF);

        return node;
    }

    //  WHILE LOOP
    if(current.type == TokenType::WHILE){

        eat(TokenType::WHILE);

        auto cond = condition();

        auto node = std::make_unique<WhileNode>(std::move(cond));

        while(current.type != TokenType::ENDWHILE){
            node->add(statement());
        }

        eat(TokenType::ENDWHILE);

        return node;
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


std::unique_ptr<ASTNode> Parser::condition(){

    auto left = expression();

    std::string op = current.value;

    if(current.type == TokenType::GREATER)
        eat(TokenType::GREATER);
    else if(current.type == TokenType::LESS)
        eat(TokenType::LESS);
    else
        throw std::runtime_error("Invalid condition");

    auto right = expression();

    return std::make_unique<BinaryNode>(
        op,
        std::move(left),
        std::move(right)
    );
}