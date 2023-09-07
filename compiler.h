#ifndef COMPILER_H
#define COMPILER_H

#include "PyCppBridge.h"

class DeepClientCppWrapper;

class HandlerParameters {
public:
    DeepClientCppWrapper* deep = nullptr;
    json data;

    HandlerParameters() {
    }

    ~HandlerParameters() {
    }

    HandlerParameters(DeepClientCppWrapper* deepClient, const std::string &jsonData) {
        deep = deepClient;
        data = json::parse(jsonData);
    }

};

class Compiler {
public:
    static std::string compileAndExecute(const std::string &code, const std::string &jwt, const std::string &gql_urn,
                                         const std::string &jsonData);
};

#endif // COMPILER_H