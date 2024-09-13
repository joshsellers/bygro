#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <string>

class Program {
public:
    void loadAndCompile(std::string filePath, bool debug = false);
private:
};

#endif