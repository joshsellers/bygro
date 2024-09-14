#include "Interpreter.h"
#include "Instructions.h"
#include <iostream>
#include "ModManager.h"
#include "Util.h"

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
            push((float)value);
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
            if ((int)pop()) {
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
            int test = (int)pop();
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
            int regAddr = (int)pop();
            float value = pop();
            _register[regAddr] = value;
            i++;
        } else if (inst == INSTRUCTION::ASSIGNSTR) {
            int regAddr = (int)pop();
            std::string str = strPop();
            _strRegister[regAddr] = str;
            i++;
        } else if (inst == INSTRUCTION::READ) {
            int regAddr = (int)pop();
            push(_register[regAddr]);
            i++;
        } else if (inst == INSTRUCTION::READSTR) {
            int regAddr = (int)pop();
            strPush(_strRegister[regAddr]);
            i++;
        } else if (inst == INSTRUCTION::ADD) {
            float right = pop();
            float left = pop();
            push(left + right);
            i++;
        } else if (inst == INSTRUCTION::SUB) {
            float right = pop();
            float left = pop();
            push(left - right);
            i++;
        } else if (inst == INSTRUCTION::MUL) {
            float right = pop();
            float left = pop();
            push(left * right);
            i++;
        } else if (inst == INSTRUCTION::DIV) {
            float right = pop();
            float left = pop();
            push(left / right);
            i++;
        } else if (inst == INSTRUCTION::MOD) {
            int right = pop();
            int left = pop();
            push(left % right);
            i++;
        } else if (inst == INSTRUCTION::GRT) {
            float right = pop();
            float left = pop();
            push((int)(left > right));
            i++;
        } else if (inst == INSTRUCTION::LSS) {
            float right = pop();
            float left = pop();
            push((int)(left < right));
            i++;
        } else if (inst == INSTRUCTION::GRE) {
            float right = pop();
            float left = pop();
            push((int)(left >= right));
            i++;
        } else if (inst == INSTRUCTION::LSE) {
            float right = pop();
            float left = pop();
            push((int)(left <= right));
            i++;
        } else if (inst == INSTRUCTION::EQL) {
            float right = pop();
            float left = pop();
            push((int)(left == right));
            i++;
        } else if (inst == INSTRUCTION::NEQ) {
            float right = pop();
            float left = pop();
            push((int)(left != right));
            i++;
        } else if (inst == INSTRUCTION::AND) {
            int right = (int)pop();
            int left = (int)pop();
            push((int)(left && right));
            i++;
        } else if (inst == INSTRUCTION::OR) {
            int right = (int)pop();
            int left = (int)pop();
            push((int)(left || right));
            i++;
        } else if (inst == INSTRUCTION::SGN) {
            float val = pop();
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
            float val = pop();
            std::cout << val;
            i++;
        } else if (inst == INSTRUCTION::NUMPRNTLN) {
            float val = pop();
            std::cout << val << std::endl;
            i++;
        } else if (inst == INSTRUCTION::CALL) {
            std::string funcName = strPop();
            push(interpret(ModManager::getFunction(funcName)));
            i++;
        } else if (inst == INSTRUCTION::POP) {
            i++;
        } else if (inst == INSTRUCTION::RAND) {
            float max = pop();
            float min = pop();
            push(randomInt(min, max));
            i++;
        } else {
            i++;
        }
    }

    return 0;
}

void Interpreter::skipStringLit(int& index, std::vector<int> bytecode) {
    int strSize = bytecode.at(index + 1);
    index += strSize + 1;
}
