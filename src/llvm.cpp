#include "llvm.h"
#include <iostream>
#include <unordered_set>

LLVMGenerator::LLVMGenerator(const std::vector<IRInstruction>& ir)
: code(ir) {}

void LLVMGenerator::generate(){

    std::cout << "\n=== LLVM IR ===\n";

    std::unordered_set<std::string> variables;

    // Step 1: collect variables
    for(auto &i : code){
        if(!i.result.empty() && isalpha(i.result[0]))
            variables.insert(i.result);
    }

    std::cout << "define i32 @main() {\n";
    std::cout << "entry:\n";

    // Step 2: allocate variables
    for(auto &v : variables){
        std::cout << "  %" << v << " = alloca i32\n";
    }

    // Step 3: generate instructions
    for(auto &i : code){

        // assignment: x = 5
        if(i.op == "="){

            std::cout << "  store i32 " << i.arg1
                      << ", i32* %" << i.result << "\n";
        }

        // arithmetic
        else if(i.op == "+" || i.op == "-" || i.op == "*" || i.op == "/"){

            std::string op;

            if(i.op == "+") op = "add";
            if(i.op == "-") op = "sub";
            if(i.op == "*") op = "mul";
            if(i.op == "/") op = "sdiv";

            std::cout << "  %tmp = " << op << " i32 "
                      << i.arg1 << ", " << i.arg2 << "\n";

            std::cout << "  store i32 %tmp, i32* %"
                      << i.result << "\n";
        }

        // print
        else if(i.op == "print"){

            std::cout << "  call void @printInt(i32 "
                      << i.arg1 << ")\n";
        }
    }

    std::cout << "  ret i32 0\n";
    std::cout << "}\n";
}