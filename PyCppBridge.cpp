#include "compiler.h"
#include "PyCppBridge.h"

bool PyPhpBridge::isAssociativeArray(const DynamicValue &cppArray) {
    return false;
}

DynamicValue PyPhpBridge::convertPyDictToCppArray(PyObject *pyDict) {
    return DynamicValue();
}

DynamicValue PyPhpBridge::convertPyListToCppArray(PyObject *pyList) {
    return DynamicValue();
}

PyObject *PyPhpBridge::convertCppArrayToPyDict(const DynamicValue &cppArray) {
    return nullptr;
}

PyObject *PyPhpBridge::convertCppArrayToPyList(const DynamicValue &cppArray) {
    return nullptr;
}

PyObject *PyPhpBridge::convertCppValueToPyObject(const DynamicValue &cppValue) {
    return nullptr;
}

std::string PyPhpBridge::getPythonErrorText() {
    return std::string();
}
