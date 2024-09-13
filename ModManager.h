#ifndef _MOD_MANAGER_H
#define _MOD_MANAGER_H

#include <map>
#include <vector>
#include <string>

class ModManager {
public:
    static void loadFunctions(bool debug = false);

    static std::vector<int> getFunction(std::string functionName);
private:
    static inline std::map<std::string, std::vector<int>> _functions;
};

#endif