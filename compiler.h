#ifndef COMPILER_H
#define COMPILER_H

#include "PyCppBridge.h"

class Compiler {
public:
    static std::string compileAndExecute(const std::string& code);
};

#endif // COMPILER_H