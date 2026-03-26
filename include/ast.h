#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <string>

class ASTNode {

public:
virtual ~ASTNode() = default;
virtual void print(int indent = 0) = 0;

};

class NumberNode : public ASTNode {

int value;

public:

NumberNode(int v) : value(v) {}

void print(int indent) override {

std::cout << std::string(indent,' ')
<< "Number(" << value << ")\n";

}

};

class VariableNode : public ASTNode {

std::string name;

public:

VariableNode(std::string n) : name(n) {}

void print(int indent) override {

std::cout << std::string(indent,' ')
<< "Variable(" << name << ")\n";

}

};

class BinaryNode : public ASTNode {

std::string op;
std::unique_ptr<ASTNode> left;
std::unique_ptr<ASTNode> right;

public:

BinaryNode(std::string o,
std::unique_ptr<ASTNode> l,
std::unique_ptr<ASTNode> r)

: op(o), left(std::move(l)), right(std::move(r)) {}

void print(int indent) override {

std::cout << std::string(indent,' ')
<< "Binary(" << op << ")\n";

left->print(indent+2);
right->print(indent+2);

}

};

class VarDeclNode : public ASTNode {

std::string name;
std::unique_ptr<ASTNode> value;

public:

VarDeclNode(std::string n,std::unique_ptr<ASTNode> v)

: name(n),value(std::move(v)) {}

void print(int indent) override {

std::cout << std::string(indent,' ')
<< "VarDecl(" << name << ")\n";

value->print(indent+2);

}

};

class PrintNode : public ASTNode {

std::unique_ptr<ASTNode> expr;

public:

PrintNode(std::unique_ptr<ASTNode> e)

: expr(std::move(e)) {}

void print(int indent) override {

std::cout << std::string(indent,' ')
<< "Print\n";

expr->print(indent+2);

}

};

class ProgramNode : public ASTNode {

std::vector<std::unique_ptr<ASTNode>> statements;

public:

void add(std::unique_ptr<ASTNode> stmt){

statements.push_back(std::move(stmt));

}

void print(int indent) override {

std::cout << "Program\n";

for(auto &s : statements)
s->print(2);

}

};