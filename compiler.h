#ifndef COMPILER_H
#define COMPILER_H

#include "DeepClientCppWrapper.h"

class HandlerParameters {
public:
    DeepClientCppWrapper deep;
    json data;

    HandlerParameters() = default;

    ~HandlerParameters() = default;

    HandlerParameters(auto deepClient, auto jsonData) {
        deep = deepClient;
        data = json::parse(jsonData);
    }

};

class Compiler {
public:
    static json compileAndExecute(const std::string &code, const std::string &jwt, const std::string &gql_urn,
                                  const std::string &jsonData);
};

#endif // COMPILER_H