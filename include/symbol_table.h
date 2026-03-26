#pragma once
#include <unordered_map>
#include <string>
#include <stdexcept>

class SymbolTable {

std::unordered_map<std::string,std::string> table;

public:

void declare(std::string name){

if(table.count(name))
throw std::runtime_error("Variable already declared: " + name);

table[name] = "int";

}

void check(std::string name){

if(!table.count(name))
throw std::runtime_error("Undeclared variable: " + name);

}

};