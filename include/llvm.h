#pragma once
#include <vector>
#include <string>
#include "ir.h"

class LLVMGenerator {

    std::vector<IRInstruction> code;

public:

    LLVMGenerator(const std::vector<IRInstruction>& ir);

    void generate();

};