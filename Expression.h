#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include <string>
#include <memory>

class Expression {
public:
    virtual ~Expression() {}
    virtual std::string evaluate() = 0;
};

class LiteralExpression : public Expression {
public:
    LiteralExpression(std::string value)
        : value(value) {}

    virtual std::string evaluate() {
        return "LIT:" + value;
    }

private:
    std::string value;
};

class MathExpression : public Expression {
public:
    MathExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right, std::string operation)
        : left(left),
          right(right),
          operation(operation) {}

    virtual std::string evaluate() {
        return left->evaluate() + ":" + right->evaluate() + ":" + operation;
    }

private:
    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;
    std::string operation;
};

class AssignmentExpression : public Expression {
public:
    AssignmentExpression(std::shared_ptr<Expression> variableName, std::shared_ptr<Expression> assignment)
        : variableName(variableName), 
          assignment(assignment)
    {}

    virtual std::string evaluate() {
        return "ASSIGN:" + variableName->evaluate() + ":" + assignment->evaluate();
    }
private:
    std::shared_ptr<Expression> variableName;
    std::shared_ptr<Expression> assignment;
};

class EngineFunctionExpression : public Expression {
public:
    EngineFunctionExpression(std::shared_ptr<Expression> function, std::shared_ptr<Expression> parameters) 
        : function(function),
          parameters(parameters)
    {}

    virtual std::string evaluate() {
        return parameters->evaluate() + ":ENFUNC:" + function->evaluate();
    }
private:
    std::shared_ptr<Expression> function;
    std::shared_ptr<Expression> parameters;
};

class ParameterExpression : public Expression {
public:
    ParameterExpression(std::shared_ptr<Expression> previousParameter, std::shared_ptr<Expression> nextParameter)
        : previousParameter(previousParameter),
          nextParameter(nextParameter)
    {}

    virtual std::string evaluate() {
        return nextParameter->evaluate() + ":" + previousParameter->evaluate();
    }
private:
    std::shared_ptr<Expression> previousParameter;
    std::shared_ptr<Expression> nextParameter;
};

#endif