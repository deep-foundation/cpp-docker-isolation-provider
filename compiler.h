#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <Python.h>
#include "httplib.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <variant>
#include <map>
#include "nlohmann/json.hpp"
#include "PyCppBridge.h"
#include "DeepClientCppWrapper.h"

using ArrayValue = std::variant<std::string, int>;

class Compiler {
public:
    static std::string compileAndExecute(const std::string& code);
};

#endif // COMPILER_H