#include "compiler.h"
#include "PyCppBridge.h"

bool PyCppBridge::isAssociativeArray(const DynamicValue &cppArray) {
    return false;
}

DynamicValue PyCppBridge::convertPyDictToCppArray(PyObject *pyDict) {
    return DynamicValue();
}

DynamicValue PyCppBridge::convertPyListToCppArray(PyObject *pyList) {
    return DynamicValue();
}

PyObject *PyCppBridge::convertCppArrayToPyDict(const DynamicValue &cppArray) {
    return nullptr;
}

PyObject *PyCppBridge::convertCppArrayToPyList(const DynamicValue &cppArray) {
    return nullptr;
}

PyObject *PyCppBridge::convertCppValueToPyObject(const DynamicValue &cppValue) {
    return nullptr;
}

std::string PyCppBridge::getPythonErrorText() {
    return std::string();
}
