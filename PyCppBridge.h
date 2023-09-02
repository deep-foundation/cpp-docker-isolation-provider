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

using json = nlohmann::json;

class DynamicValue {
public:
    virtual void print() const = 0;
    virtual json toJson() const;
    virtual ~DynamicValue() {}
};

class StringValue : public DynamicValue {
public:
    std::string value;

    StringValue(const std::string& val) : value(val) {}

    void print() const override {
        std::cout << "String value: " << value << std::endl;
    }
};

class ArrayValue : public DynamicValue {
public:
    std::variant<std::string, int> value;

    ArrayValue(const std::variant<std::string, int>& val) : value(val) {}

    void print() const override {
        std::cout << "Array value: ";
        std::visit([](const auto& v) { std::cout << v; }, value);
        std::cout << std::endl;
    }
};

class AssociativeArray : public DynamicValue {
public:
    std::map<std::string, std::shared_ptr<DynamicValue>> value;

    void print() const override {
        for (const auto& pair : value) {
            std::cout << "Key: " << pair.first << ", Value: ";
            pair.second->print();
        }
    }

    json toJson() const {
        json json_obj;
        for (const auto& pair : value) {
            json_obj[pair.first] = pair.second->toJson();
        }
        return json_obj;
    }
};

class IndexedArray : public DynamicValue {
public:
    std::vector<std::shared_ptr<DynamicValue>> value;

    void print() const override {
        for (const auto& item : value) {
            item->print();
        }
    }

    json toJson() const {
        json json_obj;
        for (size_t i = 0; i < value.size(); i++) {
            json_obj[i] = value[i]->toJson();
        }
        return json_obj;
    }
};

class IntValue : public DynamicValue {
public:
    int value;

    IntValue(int val) : value(val) {}

    void print() const override {
        std::cout << "Int value: " << value << std::endl;
    }
};

class FloatValue : public DynamicValue {
public:
    double value;

    FloatValue(double val) : value(val) {}

    void print() const override {
        std::cout << "Float value: " << value << std::endl;
    }
};

class PyCppBridge {
public:
    static bool isAssociativeArray(const std::shared_ptr<DynamicValue> &cppArray);
    static std::shared_ptr<AssociativeArray> convertPyDictToCppArray(PyObject* pyDict);
    static std::shared_ptr<IndexedArray> convertPyListToCppArray(PyObject* pyList);
    static PyObject* convertCppArrayToPyDict(const std::shared_ptr<AssociativeArray>& cppArray);
    static PyObject* convertCppArrayToPyList(const std::shared_ptr<IndexedArray>& cppArray);
    static std::string getPythonErrorText();

    static PyObject *convertCppArrayToPyObject(std::shared_ptr<DynamicValue> sharedPtr);
};


#endif //CPP_DOCKER_ISOLATION_PROVIDER_PYCPPBRIDGE_H
