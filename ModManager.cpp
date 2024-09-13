#include "ModManager.h"
#include "Util.h"
#include <filesystem>
#include <sstream>
#include <fstream>
#include <iostream>

void ModManager::loadFunctions(bool debug) {
    const std::string dirName = "functions";
    std::vector<std::string> functionFiles;
    for (const auto& entry : std::filesystem::directory_iterator(dirName)) {
        if (splitString(splitString(entry.path().string(), "\\")[1], ".").size() != 2) continue;
        else if (splitString(splitString(entry.path().string(), "\\")[1], ".")[1] != "rf") continue;
        functionFiles.push_back(splitString(entry.path().string(), "\\")[1]);
    }

    for (std::string file : functionFiles) {
        std::string inputPath = dirName + "/" + file;
        std::ifstream in(inputPath, std::ios::binary);
        std::ostringstream ostrm;
        ostrm << in.rdbuf();
        std::string inString(ostrm.str());
        in.close();

        if (debug) {
            std::cout << std::endl << "Bytecode:" << std::endl;
            for (int i = 0; i < inString.size(); i++) {
                std::cout << "0x" << std::hex << (int)inString.at(i) << " " << std::dec;
            }
            std::cout << std::endl << std::endl;
        }

        std::vector<int> bytecode;
        for (int i = 0; i < inString.size(); i++) {
            bytecode.push_back((int)inString.at(i));
        }

        std::string functionName = splitString(file, ".")[0];
        _functions[functionName] = bytecode;
    }
}

std::vector<int> ModManager::getFunction(std::string functionName) {
    if (_functions.find(functionName) == _functions.end()) {
        std::cout << "No function named: " + functionName << std::endl;
        return { 0x01, 0xFF, 0x00, 0x00, 0x00, 0x1F };
    }
    return _functions.at(functionName);
}
