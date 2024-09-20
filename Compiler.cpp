#include "Compiler.h"
#include "Util.h"
#include <map>
#include <iostream>

std::string Compiler::compile(std::string assembly) {
    std::map<std::string, INSTRUCTION> instructions = {
        {"IF",                          INSTRUCTION::IF},
        {"WHILE",                       INSTRUCTION::WHILE},
        {"BREAK",                       INSTRUCTION::BRK},
        {"CONT",                        INSTRUCTION::CONT},
        {"ENDIF",                       INSTRUCTION::ENDIF},
        {"RETURN",                      INSTRUCTION::RET},
        {"+",                           INSTRUCTION::ADD},
        {"-",                           INSTRUCTION::SUB},
        {"*",                           INSTRUCTION::MUL},
        {"/",                           INSTRUCTION::DIV},
        {"%",                           INSTRUCTION::MOD},
        {">",                           INSTRUCTION::GRT},
        {"<",                           INSTRUCTION::LSS},
        {">=",                          INSTRUCTION::GRE},
        {"<=",                          INSTRUCTION::LSE},
        {"==",                          INSTRUCTION::EQL},
        {"and",                         INSTRUCTION::AND},
        {"or",                          INSTRUCTION::OR},
        {"STWH",                        INSTRUCTION::STWH},
        {"!=",                          INSTRUCTION::NEQ},
        {"print",                       INSTRUCTION::PRNT},
        {"println",                     INSTRUCTION::PRNTLN},
        {"numPrint",                    INSTRUCTION::NUMPRNT},
        {"numPrintln",                  INSTRUCTION::NUMPRNTLN},
        {"runFunction",                 INSTRUCTION::CALL},
        {"pop",                         INSTRUCTION::POP},
        {"random",                      INSTRUCTION::RAND},
        {"displayMessage",              INSTRUCTION::DMSG},
        {"displayNumber",               INSTRUCTION::NUMDMSG},
        {"displayDebugMessage",         INSTRUCTION::DMSGDEBUG},
        {"displayWarningMessage",       INSTRUCTION::DMSGWARN},
        {"displayErrorMessage",         INSTRUCTION::DMSGERR},
        {"player.setHp",                INSTRUCTION::SPLHP},
        {"player.getHp",                INSTRUCTION::GPLHP},
        {"player.setMaxHp",             INSTRUCTION::SPLMAXHP},
        {"player.getMaxHp",             INSTRUCTION::GPLMAXHP},
        {"playSound",                   INSTRUCTION::PLAYSOUND},
        {"player.fireProjectile",       INSTRUCTION::PLSHOOT},
        {"player.giveItem",             INSTRUCTION::PLGIVEITEM},
        {"player.hasItem",              INSTRUCTION::PLHASITEM},
        {"player.weaponIsLoaded",       INSTRUCTION::PLWEAPONLOADED},
        {"player.addEffect",            INSTRUCTION::PLADDEFFECT},
        {"player.addAbility",           INSTRUCTION::PLADDABILITY},
        {"player.setAbilityParameter",  INSTRUCTION::PLSETABILITYPARAM},
        {"player.getAbilityParameter",  INSTRUCTION::PLGETABILITYPARAM},
        {"player.hasAbility",           INSTRUCTION::PLHASABILITY},
        {"player.hasEffect",            INSTRUCTION::PLHASEFFECT}
    };

    std::vector<int> bytecode;
    std::vector<std::string> parsedAssembly = splitString(assembly, ":");

    int index = 0;
    while (index < parsedAssembly.size()) {
        std::string instr = parsedAssembly.at(index);

        if (instr == "LIT") {
            std::string value = parsedAssembly.at(index + 1);
            if (stringStartsWith(value, "\"")) {
                bytecode.push_back((int)INSTRUCTION::STR);
                std::string str = value;
                replaceAll(str, "\"", "");
                int strSize = (int)str.size();
                if (strSize > 255) {
                    std::cout << "Warning: string should not be greater than 255 characters" << std::endl;
                }
                bytecode.push_back((int)strSize);

                for (int i = 0; i < str.size(); i++) {
                    bytecode.push_back((int)str.at(i));
                }
            } else if (isNumber(value)) {
                bytecode.push_back((int)INSTRUCTION::LIT);
                bytecode.push_back(std::stoi(value));
            } else if (value != "NULL") {
                bytecode.push_back((int)INSTRUCTION::LIT);
                int varIndex = 0;
                if (findStrVar(value, varIndex, false)) {
                    bytecode.push_back(varIndex);
                    bytecode.push_back((int)INSTRUCTION::READSTR);
                } else {
                    findVar(value, varIndex);
                    bytecode.push_back(varIndex);
                    bytecode.push_back((int)INSTRUCTION::READ);
                }
            }

            index += 2;
        } else if (instr == "ASSIGN") {
            std::string varName = parsedAssembly.at(index + 2);
            int varIndex = 0;
            bool isStr = false;
            if (stringEndsWith(parsedAssembly.at(index - 1), "\"")) {
                findStrVar(varName, varIndex);
                isStr = true;
            } else {
                findVar(varName, varIndex);
            }
            bytecode.push_back((int)INSTRUCTION::LIT);
            bytecode.push_back(varIndex);
            if (isStr) {
                bytecode.push_back((int)INSTRUCTION::ASSIGNSTR);
            } else {
                bytecode.push_back((int)INSTRUCTION::ASSIGN);
            }

            index += 3;
        } else {
            if (instructions.find(instr) == instructions.end()) {
                std::cout << "Error: \"" + instr + "\" is not a valid instruction" << std::endl;
                index++;
                return process(bytecode);
            }
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
        } else if ((INSTRUCTION)instr == INSTRUCTION::STR) {
            int strSize = bytecode.at(index + 1);
            for (int i = 0; i < strSize + 2; i++) {
                output += (unsigned char)(bytecode.at(index + i));
            }
            index += strSize + 2;
        } else {
            output += (unsigned char)instr;
            index++;
        }
    }

    return output;
}

bool Compiler::findVar(std::string varName, int& varIndex) {
    for (int index = 0; index < _variables.size(); index++) {
        if (_variables.at(index) == varName) {
            varIndex = index;
            return true;
        }
    }

    _variables.push_back(varName);
    varIndex = _variables.size() - 1;
    return false;
}

bool Compiler::findStrVar(std::string varName, int& varIndex, bool assignIfNotFound) {
    for (int index = 0; index < _strVariables.size(); index++) {
        if (_strVariables.at(index) == varName) {
            varIndex = index;
            return true;
        }
    }

    if (assignIfNotFound) {
        _strVariables.push_back(varName);
    }
    varIndex = _strVariables.size() - 1;
    return false;
}