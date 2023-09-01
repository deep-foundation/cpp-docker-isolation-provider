#ifndef CPP_DOCKER_ISOLATION_PROVIDER_PYCPPBRIDGE_H
#define CPP_DOCKER_ISOLATION_PROVIDER_PYCPPBRIDGE_H

#include "nlohmann/json.hpp"
#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include <Python.h>
#include "httplib.h"
#include <ctime>
#include <cstdlib>
#include <map>


class DynamicValue {
public:
    virtual void print() const = 0;
    virtual ~DynamicValue() {}
};

class StringValue : public DynamicValue {
private:
    std::string value;

public:
    StringValue(const std::string& val) : value(val) {}

    void print() const override {
        std::cout << "String value: " << value << std::endl;
    }
};

class ArrayValue : public DynamicValue {
private:
    std::variant<std::string, int> value;

public:
    ArrayValue(const std::variant<std::string, int>& val) : value(val) {}

    void print() const override {
        std::cout << "Array value: ";
        std::visit([](const auto& v) { std::cout << v; }, value);
        std::cout << std::endl;
    }
};


class PyCppBridge {
public:
    static bool isAssociativeArray(const DynamicValue& cppArray);
    static DynamicValue convertPyDictToCppArray(PyObject* pyDict);
    static DynamicValue convertPyListToCppArray(PyObject* pyList);
    static PyObject* convertCppArrayToPyDict(const DynamicValue& cppArray);
    static PyObject* convertCppArrayToPyList(const DynamicValue& cppArray);
    static PyObject* convertCppValueToPyObject(const DynamicValue& cppValue);
    static std::string getPythonErrorText();
};


#endif //CPP_DOCKER_ISOLATION_PROVIDER_PYCPPBRIDGE_H
