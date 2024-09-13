#include <iostream>

#include <sstream>
#include "Interpreter.h"
#include "Util.h"
#include <fstream>
#include "Program.h"
#include "ModManager.h"

const std::string VERSION = "1.3";

int main(int argc, char* argv[]) {
    std::cout << "bygro v" + VERSION << std::endl << std::endl;

    bool internalDebug = false;
    bool debug = false;
    bool testBuild = false;

    if (argc == 3 && argv[2] == std::string("--d")) debug = true;
    else if (argc == 3 && argv[2] == std::string("--t")) testBuild = true;
    else if (argc == 4 && argv[2] == std::string("--d") && argv[3] == std::string("--t")) {
        debug = true;
        testBuild = true;
    }

    std::string filePath = std::string(argv[1]);

    if (!testBuild) {
        Program program;
        program.loadAndCompile(filePath, debug);
    } else { 
        ModManager::loadFunctions(debug);

        srand(currentTimeMillis());
        std::cout << "Executing program" << std::endl << std::endl;
        int retVal = Interpreter::interpret(ModManager::getFunction(filePath));

        std::cout << std::endl << std::endl << "Program returned: " << std::endl << retVal;
    }

    if (internalDebug) {
        std::string outputPath = splitString(filePath, ".")[0] + ".rf";
        std::ifstream testIn(outputPath, std::ios::binary);
        std::ostringstream ostrm;
        ostrm << testIn.rdbuf();
        std::string testInString(ostrm.str());
        testIn.close();

        if (debug) {
            std::cout << std::endl << "Bytecode:" << std::endl;
            for (int i = 0; i < testInString.size(); i++) {
                std::cout << "0x" << std::hex << (int)testInString.at(i) << " " << std::dec;
            }
            std::cout << std::endl << std::endl;
        }

        std::vector<int> bytecode;
        for (int i = 0; i < testInString.size(); i++) {
            bytecode.push_back((int)testInString.at(i));
        }

        srand(currentTimeMillis());
        std::cout << "Executing program" << std::endl << std::endl;
        int retVal = Interpreter::interpret(bytecode);
        std::cout << std::endl << std::endl << "Program returned: " << std::endl << retVal;
    }
}
