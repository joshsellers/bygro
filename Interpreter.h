#ifndef _INTERPRETER_H
#define _INTERPRETER_H
#include <cassert>
#include <vector>

class Interpreter {
public:
    static void interpret(std::vector<int> bytecode);
private:
    static inline const int MAX_STACK = 128;
    static inline int _stackSize;
    static inline int _stack[MAX_STACK]; 

    static void push(int value) {
        assert(stackSize_ < MAX_STACK);
        _stack[_stackSize++] = value;
    }

    static int pop() {
        assert(_stackSize > 0);
        return _stack[--_stackSize];
    }

    static inline int _callStackSize;
    static inline int _callStack[MAX_STACK];
    static void callStackPush(int value) {
        assert(_callStackSize < MAX_STACK);
        _callStack[_callStackSize++] = value;
    }

    static int callStackPop() {
        assert(_callStackSize > 0);
        return _callStack[--_callStackSize];
    }

    static inline const int MAX_VARIABLES = 256;
    static inline int _register[MAX_VARIABLES];
};

#endif