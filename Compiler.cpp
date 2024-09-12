#include "Compiler.h"
#include "Util.h"
#include <map>

std::string Compiler::compile(std::string assembly) {
    std::map<std::string, INSTRUCTION> instructions = {
        {"IF",          INSTRUCTION::IF},
        {"WHILE",       INSTRUCTION::WHILE},
        {"BREAK",       INSTRUCTION::BRK},
        {"CONT",        INSTRUCTION::CONT},
        {"ENDIF",       INSTRUCTION::ENDIF},
        {"+",           INSTRUCTION::ADD},
        {"-",           INSTRUCTION::SUB},
        {"*",           INSTRUCTION::MUL},
        {"/",           INSTRUCTION::DIV},
        {">",           INSTRUCTION::GRT},
        {"<",           INSTRUCTION::LSS},
        {">=",          INSTRUCTION::GRE},
        {"<=",          INSTRUCTION::LSE},
        {"==",          INSTRUCTION::EQL},
        {"and",         INSTRUCTION::AND},
        {"or",          INSTRUCTION::OR},
        {"game.print",  INSTRUCTION::PRNT},
        {"STWH",        INSTRUCTION::STWH},
        {"!=",         INSTRUCTION::NEQ}
    };

    std::vector<int> bytecode;
    std::vector<std::string> parsedAssembly = splitString(assembly, ":");

    int index = 0;
    while (index < parsedAssembly.size()) {
        std::string instr = parsedAssembly.at(index);

        if (instr == "LIT") {
            std::string value = parsedAssembly.at(index + 1);
            if (isNumber(value)) {
                bytecode.push_back((int)INSTRUCTION::LIT);
                bytecode.push_back(std::stoi(value));
            } else if (value != "NULL") {
                bytecode.push_back((int)INSTRUCTION::LIT);
                int varIndex = findVar(value);
                bytecode.push_back(varIndex);
                bytecode.push_back((int)INSTRUCTION::READ);
            }

            index += 2;
        } else if (instr == "ASSIGN") {
            std::string varName = parsedAssembly.at(index + 2);
            int varIndex = findVar(varName);
            bytecode.push_back((int)INSTRUCTION::LIT);
            bytecode.push_back(varIndex);
            bytecode.push_back((int)INSTRUCTION::ASSIGN);

            index += 3;
        } else {
            bytecode.push_back((int)instructions.at(instr));
            index++;
        }
    }

    return process(bytecode);
}

std::string Compiler::process(std::vector<int> bytecode) {
    std::string output = "";
    int index = 0;

    while (index < bytecode.size()) {
        int instr = bytecode.at(index);
        
        if ((INSTRUCTION)instr == INSTRUCTION::LIT) {
            output += (unsigned char)instr;
            int literal = bytecode.at(index + 1);

            for (int i = 0; i < 4; i++) {
                output += (unsigned char)(literal >> (i * 8));
            }

            index += 2;
        } else {
            output += (unsigned char)instr;
            index++;
        }
    }

    return output;
}

int Compiler::findVar(std::string varName) {
    for (int index = 0; index < _variables.size(); index++) {
        if (_variables.at(index) == varName) return index;
    }

    _variables.push_back(varName);
    return _variables.size() - 1;
}