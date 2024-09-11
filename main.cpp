#include <iostream>
#include "Tokenizer.h"
#include <map>
#include "Expression.h"
#include "Parser.h"
#include <fstream>
#include <functional>

int main(int argc, char* argv[]) {
    std::string filePath = std::string(argv[1]);
    std::ifstream in(filePath);
    std::string rawProgram = "";
    if (in.good()) {
        std::string line;
        while (getline(in, line)) {
            if (stringStartsWith(line, "#")) continue;
            line.erase(std::find_if(line.rbegin(), line.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), line.end());
            rawProgram += line + " ";
        }
    }
    rawProgram.erase(std::find_if(rawProgram.rbegin(), rawProgram.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), rawProgram.end());

    std::vector<std::string> tokens = Tokenizer::tokenize(rawProgram);

    std::vector<std::vector<std::string>> statements;
    std::vector<std::string> currentStatement;
    for (std::string token : tokens) {
        if (token != ";") currentStatement.push_back(token);
        else {
            statements.push_back(currentStatement);
            currentStatement.clear();
        }
    }

    Parser parser;
    std::vector<EXPR> expressions = parser.parse(tokens);

    std::string assembly = "";
    for (auto& expression : expressions) {
        assembly += expression->evaluate() + "\n";
    }

    std::cout << assembly << std::endl;
}
