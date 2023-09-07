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
    virtual json toJson() const = 0;
    virtual ~DynamicValue() {}
};

class StringValue : public DynamicValue {
public:
    std::string cppValue;

    StringValue(const std::string& val) : cppValue(val) {}

    void print() const override {
        std::cout << "String value: " << cppValue << std::endl;
    }

    json toJson() const override {
        return
                cppValue
        ;
    }
};

class NoneValue : public DynamicValue {
public:
    std::string cppValue;

    NoneValue() = default;

    void print() const override {
        std::cout << "None" << std::endl;
    }

    json toJson() const override {
        return
                "none"
        ;
    }
};

class ArrayValue : public DynamicValue {
public:
    std::variant<std::string, int> cppValue;

    ArrayValue(const std::variant<std::string, int>& val) : cppValue(val) {}

    void print() const override {
        std::cout << "Array value: ";
        std::visit([](const auto& v) { std::cout << v; }, cppValue);
        std::cout << std::endl;
    }

    json toJson() const override {
        json result;
        if (std::holds_alternative<std::string>(cppValue)) {
            result["type"] = "string";
            result["value"] = std::get<std::string>(cppValue);
        } else if (std::holds_alternative<int>(cppValue)) {
            result["type"] = "int";
            result["value"] = std::get<int>(cppValue);
        }
        return result;
    }
};

class AssociativeArray : public DynamicValue {
public:
    std::map<std::string, std::shared_ptr<DynamicValue>> cppValue;

    void print() const override {
        for (const auto& pair : cppValue) {
            std::cout << "Key: " << pair.first << ", value: ";
            pair.second->print();
        }
    }

    json toJson() const {
        json json_obj;
        for (const auto& pair : cppValue) {
            json_obj[pair.first] = pair.second->toJson();
        }
        return json_obj;
    }
};

class IndexedArray : public DynamicValue {
public:
    std::vector<std::shared_ptr<DynamicValue>> cppValue;

    void print() const override {
        for (const auto& item : cppValue) {
            item->print();
        }
    }

    json toJson() const {
        json json_obj;
        for (size_t i = 0; i < cppValue.size(); i++) {
            json_obj[i] = cppValue[i]->toJson();
        }
        return json_obj;
    }
};

class IntValue : public DynamicValue {
public:
    int cppValue;

    IntValue(int val) : cppValue(val) {}

    void print() const override {
        std::cout << "Int value: " << cppValue << std::endl;
    }

    json toJson() const override {
        return
                cppValue
        ;
    }
};

class FloatValue : public DynamicValue {
public:
    double cppValue;

    FloatValue(double val) : cppValue(val) {}

    void print() const override {
        std::cout << "Float value: " << cppValue << std::endl;
    }

    json toJson() const override {
        return
                cppValue
        ;
    }
};

class PyCppBridge {
public:
    static std::shared_ptr<AssociativeArray> convertPyDictToCppArray(PyObject* pyDict);
    static std::shared_ptr<IndexedArray> convertPyListToCppArray(PyObject* pyList);
    static PyObject* convertCppArrayToPyDict(const std::shared_ptr<AssociativeArray>& cppArray);
    static PyObject* convertCppArrayToPyList(const std::shared_ptr<IndexedArray>& cppArray);
    static std::string getPythonErrorText();

    static PyObject *convertCppArrayToPyObject(const std::shared_ptr<DynamicValue>& sharedPtr);
};


#endif //CPP_DOCKER_ISOLATION_PROVIDER_PYCPPBRIDGE_H
