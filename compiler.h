#ifndef COMPILER_H
#define COMPILER_H

#include "PyCppBridge.h"

class Compiler {
public:
    static std::string compileAndExecute(const std::string &code, const std::string &jwt, const std::string &gql_urn);
};

#endif // COMPILER_H