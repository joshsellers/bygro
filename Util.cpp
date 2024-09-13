#include "Util.h"
#include <chrono>

std::string trimString(std::string str) {
    if (str.find(".") != std::string::npos) {
        for (std::string::size_type s = str.length() - 1; s > 0; --s) {
            if (str[s - 1] == '.') break;
            else if (str[s] == '0') str.erase(s, 1);
            else break;
        }
    }
    return str;
}

std::vector<std::string> splitString(std::string str, std::string delimiter) {
    size_t pos = 0;
    std::vector<std::string> parsedString;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        parsedString.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    parsedString.push_back(str);

    return parsedString;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

bool stringStartsWith(std::string str, std::string start) {
    return str.rfind(start, 0) == 0;
}

bool stringEndsWith(std::string const& fullString, std::string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

bool isNumber(std::string s) {
    return !s.empty() && s.find_first_not_of("0123456789.") == std::string::npos;
}

void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

int randomInt(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

long long currentTimeMillis() {
    auto time = std::chrono::system_clock::now();

    auto since_epoch = time.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>
        (since_epoch);

    return millis.count();
}