#include "Program.h"
#include "Tokenizer.h"
#include <map>
#include "Expression.h"
#include "Parser.h"
#include <fstream>
#include <functional>
#include "Compiler.h"
#include <filesystem>
#include <iostream>

void Program::loadAndCompile(std::string filePath, bool debug) {
    std::ifstream in(filePath);
    std::string rawProgram = "";
    if (in.good()) {
        std::string line;
        while (getline(in, line)) {
            bool foundNonComment = false;
            bool shouldSkipLine = false;
            for (auto& character : line) {
                if (character == ' ') continue;
                else if (character == '#' && !foundNonComment) {
                    shouldSkipLine = true;
                    break;
                } else {
                    foundNonComment = true;
                    break;
                }
            }
            if (shouldSkipLine) continue;
            rtrim(line);
            rawProgram += line + " ";
        }
    }
    rtrim(rawProgram);

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
        assembly += expression->evaluate() + ":";
    }
    replaceAll(assembly, "::", ":");
    if (stringEndsWith(assembly, ":")) {
        assembly.pop_back();
    }

    if (debug) {
        std::cout << "Assembly:" << std::endl;
        std::cout << assembly << std::endl << std::endl;
    }

    std::string output = Compiler::compile(assembly);

    std::string outputPath = splitString(filePath, ".")[0] + ".rf";
    try {
        if (!std::filesystem::remove(outputPath)) {
            std::cout << "Could not replace output file" << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& err) {
        std::cout << "Could not replace output file:\n" << err.what() << std::endl;
    }

    try {
        std::ofstream out(outputPath, std::ios::binary);
        out << output;
        out.close();
    } catch (std::exception ex) {
        std::cout << "Error writing to output file:\n" << ex.what() << std::endl;
    }

    std::cout << "Build successful" << std::endl << std::endl;
}
