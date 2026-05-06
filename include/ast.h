#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <string>

inline void printIndent(int indent){
    for(int i = 0; i < indent; i++)
        std::cout << "  ";
}

class ASTNode {

    public:
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) = 0;

};

class NumberNode : public ASTNode {

public:

    int value;

    NumberNode(int v) : value(v) {}

    void print(int indent) override {
        
        printIndent(indent);
        std::cout << "Number(" << value << ")\n";

    }

};

class VariableNode : public ASTNode {

public:

    std::string name;

    VariableNode(std::string n) : name(n) {}

    void print(int indent) override {
        
        printIndent(indent);
        std::cout << "Variable(" << name << ")\n";

    }

};

class BinaryNode : public ASTNode {

public:

    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryNode(std::string o,
    std::unique_ptr<ASTNode> l,
    std::unique_ptr<ASTNode> r)

    : op(o), left(std::move(l)), right(std::move(r)) {}

    void print(int indent) override {
        
        printIndent(indent);
        std::cout << "Binary(" << op << ")\n";

        left->print(indent+1);
        right->print(indent+1);

    }

};

class VarDeclNode : public ASTNode {

public:

    std::string name;
    std::unique_ptr<ASTNode> value;

    VarDeclNode(std::string n,std::unique_ptr<ASTNode> v)

    : name(n),value(std::move(v)) {}

    void print(int indent) override {

        printIndent(indent);
        std::cout << "VarDecl(" << name << ")\n";

        value->print(indent+1);

    }

};

class PrintNode : public ASTNode {

public:

    std::unique_ptr<ASTNode> expr;

    PrintNode(std::unique_ptr<ASTNode> e)

    : expr(std::move(e)) {}

    void print(int indent) override {

        printIndent(indent);
        std::cout << "Print\n";

        expr->print(indent+1);

    }

};

class ProgramNode : public ASTNode {

public:

    std::vector<std::unique_ptr<ASTNode>> statements;

    void add(std::unique_ptr<ASTNode> stmt){

        statements.push_back(std::move(stmt));

    }

    void print(int indent) override {

        printIndent(indent);
        std::cout << "Program\n";

        for(auto &s : statements)
            s->print(indent + 1);

    }

};

class AssignmentNode : public ASTNode {

public:
    std::string name;
    std::unique_ptr<ASTNode> value;

    AssignmentNode(std::string n, std::unique_ptr<ASTNode> v)
    : name(n), value(std::move(v)) {}

    void print(int indent) override {

        printIndent(indent);
        std::cout << "Assign(" << name << ")\n";

        value->print(indent+1);
    }
};

class IfNode : public ASTNode {

public:
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> body;

    IfNode(std::unique_ptr<ASTNode> cond)
    : condition(std::move(cond)) {}

    void add(std::unique_ptr<ASTNode> stmt){
        body.push_back(std::move(stmt));
    }

    void print(int indent) override {

        printIndent(indent);
        std::cout << "If\n";

        condition->print(indent+1);

        for(auto &s : body)
            s->print(indent+1);
    }
};

class WhileNode : public ASTNode {

public:
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> body;

    WhileNode(std::unique_ptr<ASTNode> cond)
    : condition(std::move(cond)) {}

    void add(std::unique_ptr<ASTNode> stmt){
        body.push_back(std::move(stmt));
    }

    void print(int indent) override {

        printIndent(indent);
        std::cout << "While\n";

        condition->print(indent + 1);

        for(auto &s : body)
            s->print(indent + 1);
    }
};