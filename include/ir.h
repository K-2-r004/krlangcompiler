#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "ast.h"
#include <unordered_map>
#include <unordered_set>

struct IRInstruction {

    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;

};

class IRGenerator {

    int tempCount;
    std::vector<IRInstruction> code;
    std::unordered_map<std::string, std::string> constTable;
    std::string newTemp();

public:

    IRGenerator();

    std::string generate(ASTNode* node);

    std::vector<IRInstruction> getCode() const { return code; }
    
    void deadCodeElimination();
    void print();

};