#include "compiler.h"

bool PyCppBridge::isAssociativeArray(const DynamicValue &cppArray) {
    return false;
}

DynamicValue PyCppBridge::convertPyDictToCppArray(PyObject *pyDict) {
    DynamicValue cppArray;

    PyObject* pyKey;
    PyObject* pyValue;
    Py_ssize_t pos = 0;

    while (PyDict_Next(pyDict, &pos, &pyKey, &pyValue)) {
        std::string key(PyUnicode_AsUTF8(pyKey));

        if (PyLong_Check(pyValue)) {
            cppArray[key] = PyLong_AsLong(pyValue);
        } else if (PyFloat_Check(pyValue)) {
            cppArray[key] = PyFloat_AsDouble(pyValue);
        } else if (PyUnicode_Check(pyValue)) {
            cppArray[key] = std::string(PyUnicode_AsUTF8(pyValue));
        } else if (PyDict_Check(pyValue)) {
            cppArray[key] = convertPyDictToCppArray(pyValue);
        } else if (PyList_Check(pyValue)) {
            cppArray[key] = convertPyListToCppArray(pyValue);
        } else {
            cppArray[key] = nullptr; // Handle unsupported types as nullptr or customize as needed.
        }
    }

    return cppArray;
}

DynamicValue PyCppBridge::convertPyListToCppArray(PyObject *pyList) {
    DynamicValue cppArray;

    Py_ssize_t size = PyList_Size(pyList);
    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject* pyValue = PyList_GetItem(pyList, i);

        if (PyLong_Check(pyValue)) {
            cppArray[i] = PyLong_AsLong(pyValue);
        } else if (PyFloat_Check(pyValue)) {
            cppArray[i] = PyFloat_AsDouble(pyValue);
        } else if (PyUnicode_Check(pyValue)) {
            cppArray[i] = std::string(PyUnicode_AsUTF8(pyValue));
        } else if (PyList_Check(pyValue)) {
            cppArray[i] = convertPyListToCppArray(pyValue);
        } else if (PyDict_Check(pyValue)) {
            cppArray[i] = convertPyDictToCppArray(pyValue);
        } else {
            cppArray[i] = nullptr; // Handle unsupported types as nullptr or customize as needed.
        }
    }

    return cppArray;
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
    PyObject *exc_type, *exc_value, *exc_traceback;
    PyErr_Fetch(&exc_type, &exc_value, &exc_traceback);

    if (exc_value) {
        PyObject *str_exc_value = PyObject_Str(exc_value);
        if (str_exc_value) {
            const char *c_str = PyUnicode_AsUTF8(str_exc_value);
            std::string errorText(c_str);
            Py_XDECREF(str_exc_value);
            PyErr_Restore(exc_type, exc_value, exc_traceback);
            return errorText;
        }
    }

    PyErr_Restore(exc_type, exc_value, exc_traceback);
    return "Unknown runtime error";
}
