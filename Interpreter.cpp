#include "Interpreter.h"
#include "Instructions.h"
#include <iostream>
#include "ModManager.h"

int Interpreter::interpret(std::vector<int> bytecode) {
    int i = 0;
    while (i < bytecode.size()) {
        INSTRUCTION inst = (INSTRUCTION)bytecode.at(i);

        if (inst == INSTRUCTION::LIT) {
            int value = 0;
            for (int j = 3; j >= 0; j--) {
                int byte = bytecode.at((i + 1) + j);
                value += byte << (j * 8);
            }
            push(value);
            i += 5;
        } else if (inst == INSTRUCTION::STR) {
            std::string str = "";
            int strSize = bytecode.at(i + 1);
            for (int j = 0; j < strSize; j++) {
                str += (char)bytecode.at(i + 2 + j);
            }
            strPush(str);
            i += strSize + 2;
        } else if (inst == INSTRUCTION::IF) {
            if (pop()) {
                callStackPush(-1);
                i++;
            } else {
                int ifWhileCount = 0;
                int endIfCount = 0;
                for (int j = i + 1; j < bytecode.size(); j++) {
                    INSTRUCTION subInst = (INSTRUCTION)bytecode.at(j);
                    if (subInst == INSTRUCTION::LIT) j += 4;
                    else if (subInst == INSTRUCTION::STR) skipStringLit(j, bytecode);
                    else if (subInst == INSTRUCTION::WHILE || subInst == INSTRUCTION::IF) ifWhileCount++;
                    else if (subInst == INSTRUCTION::ENDIF) endIfCount++;
                    if (endIfCount > ifWhileCount) {
                        i = j + 1;
                        break;
                    }
                }
            }
        } else if (inst == INSTRUCTION::ENDIF) {
            int addr = callStackPop();
            if (addr == -1) i++;
            else {
                i = addr;
            }
        } else if (inst == INSTRUCTION::WHILE) {
            int test = pop();
            if (test) {
                i++;
            } else {
                int ifWhileCount = 0;
                int endIfCount = 0;
                for (int j = i + 1; j < bytecode.size(); j++) {
                    INSTRUCTION subInst = (INSTRUCTION)bytecode.at(j);
                    if (subInst == INSTRUCTION::LIT) j += 4;
                    else if (subInst == INSTRUCTION::STR) skipStringLit(j, bytecode);
                    else if (subInst == INSTRUCTION::WHILE || subInst == INSTRUCTION::IF) ifWhileCount++;
                    else if (subInst == INSTRUCTION::ENDIF) endIfCount++;
                    if (endIfCount > ifWhileCount) {
                        callStackPop();
                        i = j + 1;
                        break;
                    }
                }
            }
        } else if (inst == INSTRUCTION::STWH) {
            callStackPush(i);
            i++;
        } else if (inst == INSTRUCTION::BRK) {
            for (int j = i + 1; j < bytecode.size(); j++) {
                INSTRUCTION subInst = (INSTRUCTION)bytecode.at(j);

                int currentAddr = -1;
                if (subInst == INSTRUCTION::LIT) j += 4;
                else if (subInst == INSTRUCTION::STR) skipStringLit(j, bytecode);
                else if (subInst == INSTRUCTION::WHILE || subInst == INSTRUCTION::IF) callStackPush(-1);
                else if (subInst == INSTRUCTION::ENDIF) {
                    currentAddr = callStackPop();
                }

                if (currentAddr != -1) {
                    i = j + 1;
                    break;
                }
            }
        } else if (inst == INSTRUCTION::CONT) {
            i = callStackPop();
            while (i == -1) {
                i = callStackPop();
            }
        } else if (inst == INSTRUCTION::RET) {
            return pop();
        } else if (inst == INSTRUCTION::ASSIGN) {
            int regAddr = pop();
            int value = pop();
            _register[regAddr] = value;
            i++;
        } else if (inst == INSTRUCTION::ASSIGNSTR) {
            int regAddr = pop();
            std::string str = strPop();
            _strRegister[regAddr] = str;
            i++;
        } else if (inst == INSTRUCTION::READ) {
            int regAddr = pop();
            push(_register[regAddr]);
            i++;
        } else if (inst == INSTRUCTION::READSTR) {
            int regAddr = pop();
            strPush(_strRegister[regAddr]);
            i++;
        } else if (inst == INSTRUCTION::ADD) {
            int right = pop();
            int left = pop();
            push(left + right);
            i++;
        } else if (inst == INSTRUCTION::SUB) {
            int right = pop();
            int left = pop();
            push(left - right);
            i++;
        } else if (inst == INSTRUCTION::MUL) {
            int right = pop();
            int left = pop();
            push(left * right);
            i++;
        } else if (inst == INSTRUCTION::DIV) {
            int right = pop();
            int left = pop();
            push(left / right);
            i++;
        } else if (inst == INSTRUCTION::MOD) {
            int right = pop();
            int left = pop();
            push(left % right);
            i++;
        } else if (inst == INSTRUCTION::GRT) {
            int right = pop();
            int left = pop();
            push(left > right);
            i++;
        } else if (inst == INSTRUCTION::LSS) {
            int right = pop();
            int left = pop();
            push(left < right);
            i++;
        } else if (inst == INSTRUCTION::GRE) {
            int right = pop();
            int left = pop();
            push(left >= right);
            i++;
        } else if (inst == INSTRUCTION::LSE) {
            int right = pop();
            int left = pop();
            push(left <= right);
            i++;
        } else if (inst == INSTRUCTION::EQL) {
            int right = pop();
            int left = pop();
            push(left == right);
            i++;
        } else if (inst == INSTRUCTION::NEQ) {
            int right = pop();
            int left = pop();
            push(left != right);
            i++;
        } else if (inst == INSTRUCTION::AND) {
            int right = pop();
            int left = pop();
            push(left && right);
            i++;
        } else if (inst == INSTRUCTION::OR) {
            int right = pop();
            int left = pop();
            push(left || right);
            i++;
        } else if (inst == INSTRUCTION::SGN) {
            int val = pop();
            push(-val);
            i++;
        } else if (inst == INSTRUCTION::PRNT) {
            std::string val = strPop();
            std::cout << val;
            i++;
        } else if (inst == INSTRUCTION::PRNTLN) {
            std::string val = strPop();
            std::cout << val << std::endl;
            i++;
        } else if (inst == INSTRUCTION::NUMPRNT) {
            int val = pop();
            std::cout << val;
            i++;
        } else if (inst == INSTRUCTION::NUMPRNTLN) {
            int val = pop();
            std::cout << val << std::endl;
            i++;
        } else if (inst == INSTRUCTION::CALL) {
            std::string funcName = strPop();
            push(interpret(ModManager::getFunction(funcName)));
            i++;
        } else if (inst == INSTRUCTION::POP) {
            i++;
        }
    }

    return 0;
}

void Interpreter::skipStringLit(int& index, std::vector<int> bytecode) {
    int strSize = bytecode.at(index + 1);
    index += strSize + 1;
}
