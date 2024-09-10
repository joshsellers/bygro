#include "Tokenizer.h"
#include "Util.h"

std::vector<std::string> Tokenizer::tokenize(std::string inScript) {
    std::string script = "";
    bool replaceSpaces = false;
    for (auto symbol : inScript) {
        if (replaceSpaces && symbol == ' ') {
            script += "RPLSPC";
        } else {
            script += std::string(1, symbol);
        }
        if (symbol == '"') replaceSpaces = !replaceSpaces;
    }

    std::vector<std::string> tokens;
    
    std::vector<std::string> bareTokens = splitString(script, " ");
    for (std::string& bareToken : bareTokens) {
        replaceAll(bareToken, "RPLSPC", " ");
    }

    for (std::string bareToken : bareTokens) {
        std::vector<std::string> operatorTokens = splitOperators(bareToken);
        for (std::string token : operatorTokens) {
            if (token != "") 
                tokens.push_back(token);
        }
    }

    return tokens;
}

std::vector<std::string> Tokenizer::splitOperators(std::string bareToken) {
    std::vector<std::string> operatorExpressionTokens;
    std::vector<std::string> operators = { ".", "+", "-", "*", "/", "=", ";", ",", ":"};
    for (std::string operatorString : operators) {
        if (bareToken.find(operatorString) != std::string::npos) {
            std::vector<std::string> tokens = splitString(bareToken, operatorString);
            std::vector<std::string> previousTokens = splitOperators(tokens.at(0));
            for (std::string previousToken : previousTokens) {
                operatorExpressionTokens.push_back(previousToken);
            }
            
            std::vector<std::string> additionalExpressions;
            for (int i = 1; i < tokens.size(); i++) {
                additionalExpressions.push_back(operatorString);
                for (std::string subToken : splitOperators(tokens.at(i))) {
                    additionalExpressions.push_back(subToken);
                }
            }

            for (std::string expression : additionalExpressions) {
                operatorExpressionTokens.push_back(expression);
            }

            return operatorExpressionTokens;
        }
    }

    operatorExpressionTokens.push_back(bareToken);
    return operatorExpressionTokens;
}
