#ifndef CPP_DOCKER_ISOLATION_PROVIDER_PYCPPBRIDGE_H
#define CPP_DOCKER_ISOLATION_PROVIDER_PYCPPBRIDGE_H

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

class PyPhpBridge {
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
