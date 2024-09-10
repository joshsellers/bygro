#include <iostream>
#include "Tokenizer.h"
#include <map>
#include "Expression.h"

std::shared_ptr<Expression> evaluateStatement(std::vector<std::string> statement, int from, std::shared_ptr<Expression> previousExpression) {
    for (int i = from; i < statement.size(); i++) {
        std::string token = statement.at(i);
        if (token == "=") {
            std::shared_ptr<Expression> variableName = previousExpression;
            std::shared_ptr<Expression> assignment = evaluateStatement(statement, i + 1, nullptr);
            std::shared_ptr<AssignmentExpression> expr = std::shared_ptr<AssignmentExpression>(new AssignmentExpression(variableName, assignment));
            return expr;
        } else if (token == "+" || token == "-" || token == "*" || token == "/") {
            std::shared_ptr<Expression> left = previousExpression;
            std::shared_ptr<Expression> right = evaluateStatement(statement, i + 1, nullptr);
            std::shared_ptr<MathExpression> expr = std::shared_ptr<MathExpression>(new MathExpression(left, right, token));
            return expr;
        } else if (token == ":") {
            std::shared_ptr<Expression> function = previousExpression;
            std::shared_ptr<Expression> parameters = evaluateStatement(statement, i + 1, nullptr);
            std::shared_ptr<EngineFunctionExpression> expr = std::shared_ptr<EngineFunctionExpression>(new EngineFunctionExpression(function, parameters));
            return expr;
        } else if (token == ",") {
            std::shared_ptr<ParameterExpression> parameters = std::shared_ptr<ParameterExpression>(
                new ParameterExpression(previousExpression, evaluateStatement(statement, i + 1, previousExpression)));
            return parameters;
        } else {
            std::shared_ptr<LiteralExpression> expr = std::shared_ptr<LiteralExpression>(new LiteralExpression(token));
            return evaluateStatement(statement, i + 1, expr);
        }
    }

    return previousExpression;
}

int main() {
    std::string test = "value = 1 + 2; giveAbility: \" te st\", value, 2;";
    std::vector<std::string> tokens = Tokenizer::tokenize(test);

    std::vector<std::vector<std::string>> statements;
    std::vector<std::string> currentStatement;
    for (std::string token : tokens) {
        if (token != ";") currentStatement.push_back(token);
        else {
            statements.push_back(currentStatement);
            currentStatement.clear();
        }
    }

    std::vector<std::shared_ptr<Expression>> expressions;
    for (auto& statement : statements) {
        expressions.push_back(evaluateStatement(statement, 0, nullptr));
    }

    std::string assembly = "";
    for (auto& expression : expressions) {
        assembly += expression->evaluate() + "\n";
    }

    std::cout << assembly << std::endl;
}
