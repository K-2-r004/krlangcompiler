#include "ir.h"
#include "ast.h"
#include <string>
#include <algorithm>

bool isNumber(const std::string& s){
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

IRGenerator::IRGenerator() : tempCount(0) {}

std::string IRGenerator::newTemp() {
    return "t" + std::to_string(tempCount++);
}
std::string IRGenerator::generate(ASTNode* node) {

    
    if(auto num = dynamic_cast<NumberNode*>(node)) {
        return std::to_string(num->value);
    }

    
    if(auto var = dynamic_cast<VariableNode*>(node)) {

       
        if(constTable.count(var->name))
            return constTable[var->name];

        return var->name;
    }

    
    if(auto bin = dynamic_cast<BinaryNode*>(node)) {

        std::string left = generate(bin->left.get());
        std::string right = generate(bin->right.get());

        if(isNumber(left) && isNumber(right)) {

            int l = stoi(left);
            int r = stoi(right);
            int result = 0;

            if(bin->op == "+") result = l + r;
            else if(bin->op == "-") result = l - r;
            else if(bin->op == "*") result = l * r;
            else if(bin->op == "/") result = l / r;
            else if(bin->op == ">") result = l > r;
            else if(bin->op == "<") result = l < r;

            return std::to_string(result);
        }


        std::string temp = newTemp();

        code.push_back({
            bin->op,
            left,
            right,
            temp
        });

        return temp;
    }

    if(auto pr = dynamic_cast<PrintNode*>(node)) {

        std::string val = generate(pr->expr.get());

        code.push_back({
            "print",
            val,
            "",
            ""
        });

        return "";
    }

    if(auto vd = dynamic_cast<VarDeclNode*>(node)) {

        std::string val = generate(vd->value.get());

        if(!val.empty() && isNumber(val)) {
            constTable[vd->name] = val;
        } else {
            constTable.erase(vd->name);
        }

        code.push_back({
            "=",
            val,
            "",
            vd->name
        });

        return vd->name;
    }


    if(auto asg = dynamic_cast<AssignmentNode*>(node)) {

        std::string val = generate(asg->value.get());

        // Track constant values
        if(!val.empty() && isNumber(val)) {
            constTable[asg->name] = val;
        } else {
            constTable.erase(asg->name);
        }

        code.push_back({
            "=",
            val,
            "",
            asg->name
        });

        return asg->name;
    }

    if(auto ifnode = dynamic_cast<IfNode*>(node)){

        std::string cond = generate(ifnode->condition.get());

        std::string label = "L" + std::to_string(tempCount++);

        code.push_back({"ifFalse", cond, "", label});

        for(auto &stmt : ifnode->body)
            generate(stmt.get());

        code.push_back({"label", "", "", label});

        return "";
    }
    


    if(auto prog = dynamic_cast<ProgramNode*>(node)) {

        for(auto &stmt : prog->statements)
            generate(stmt.get());

        return "";
    }

    if(auto wh = dynamic_cast<WhileNode*>(node)){

        std::string startLabel = "L" + std::to_string(tempCount++);
        std::string endLabel = "L" + std::to_string(tempCount++);

    
        code.push_back({"label","","",startLabel});

        
        std::string cond = generate(wh->condition.get());

        code.push_back({"ifFalse", cond, "", endLabel});

        
        for(auto &stmt : wh->body)
            generate(stmt.get());

        
        code.push_back({"goto","","",startLabel});

        
        code.push_back({"label","","",endLabel});

        return "";
    }

    return "";
}
void IRGenerator::print() {

    std::cout << "\n=== THREE ADDRESS CODE ===\n";

    for(auto &i : code) {

        if(i.op == "print") {
            std::cout << "print " << i.arg1 << "\n";
        }
        else if(i.op == "=") {
            std::cout << i.result << " = " << i.arg1 << "\n";
        }
        else if(i.op == "ifFalse"){
            std::cout << "ifFalse " << i.arg1 << " goto " << i.result << "\n";
        }
        else if(i.op == "label"){
            std::cout << i.result << ":\n";
        }
        else if(i.op == "goto"){
            std::cout << "goto " << i.result << "\n";
        }
        else {
            std::cout
            << i.result << " = "
            << i.arg1 << " "
            << i.op << " "
            << i.arg2 << "\n";
        }
    }
}

void IRGenerator::deadCodeElimination(){

    std::unordered_set<std::string> used;
    std::vector<IRInstruction> newCode;

    
    for(auto &i : code){
        if(i.op == "print"){
            used.insert(i.arg1);
        }
    }

    
    for(int i = code.size() - 1; i >= 0; i--){

        auto &inst = code[i];

        if(inst.op == "=" || inst.op == "+" || inst.op == "-" ||
           inst.op == "*" || inst.op == "/" ||
           inst.op == "<" || inst.op == ">"){

            if(used.count(inst.result) || inst.result[0] != 't'){

                
                newCode.push_back(inst);

                
                if(!inst.arg1.empty()) used.insert(inst.arg1);
                if(!inst.arg2.empty()) used.insert(inst.arg2);
            }
        }
        else{
            
            newCode.push_back(inst);

            if(!inst.arg1.empty()) used.insert(inst.arg1);
        }
    }

    
    std::reverse(newCode.begin(), newCode.end());
    code = newCode;
}