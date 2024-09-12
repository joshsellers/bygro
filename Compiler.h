#ifndef _COMPILER_H
#define _COMPILER_H

#include <string>
#include <vector>
#include "Instructions.h"

constexpr int MAX_VARIABLES = 256;
class Compiler {
public:
    static std::string compile(std::string assembly);
private:
    static std::string process(std::vector<int> bytecode);

    static inline std::vector<std::string> _variables;
    static int findVar(std::string varName);
};

#endif
