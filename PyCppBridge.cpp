#include "compiler.h"

bool PyCppBridge::isAssociativeArray(const DynamicValue &cppArray) {
    return false;
}

DynamicValue PyCppBridge::convertPyDictToCppArray(PyObject *pyDict) {
    auto cppArray = std::make_shared<AssociativeArray>();

    PyObject* pyKey;
    PyObject* pyValue;
    Py_ssize_t pos = 0;

    while (PyDict_Next(pyDict, &pos, &pyKey, &pyValue)) {
        std::string key(PyUnicode_AsUTF8(pyKey));

        if (PyLong_Check(pyValue)) {
            cppArray->value[key] = std::make_shared<IntValue>(PyLong_AsLong(pyValue));
        } else if (PyFloat_Check(pyValue)) {
            cppArray->value[key] = std::make_shared<FloatValue>(PyFloat_AsDouble(pyValue));
        } else if (PyUnicode_Check(pyValue)) {
            cppArray->value[key] = std::make_shared<StringValue>(PyUnicode_AsUTF8(pyValue));
        } else if (PyDict_Check(pyValue)) {
            cppArray->value[key] = convertPyDictToCppArray(pyValue);
        } else if (PyList_Check(pyValue)) {
            cppArray->value[key] = convertPyListToCppArray(pyValue);
        } else {
            // Handle unsupported types as needed.
            cppArray->value[key] = nullptr;
        }
    }

    return cppArray;
}

DynamicValue PyCppBridge::convertPyListToCppArray(PyObject *pyList) {
    auto cppArray = std::make_shared<IndexedArray>();

    Py_ssize_t size = PyList_Size(pyList);
    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject* pyValue = PyList_GetItem(pyList, i);

        if (PyLong_Check(pyValue)) {
            cppArray->value.push_back(std::make_shared<IntValue>(PyLong_AsLong(pyValue)));
        } else if (PyFloat_Check(pyValue)) {
            cppArray->value.push_back(std::make_shared<FloatValue>(PyFloat_AsDouble(pyValue)));
        } else if (PyUnicode_Check(pyValue)) {
            cppArray->value.push_back(std::make_shared<StringValue>(PyUnicode_AsUTF8(pyValue)));
        } else if (PyList_Check(pyValue)) {
            cppArray->value.push_back(convertPyListToCppArray(pyValue));
        } else if (PyDict_Check(pyValue)) {
            cppArray->value.push_back(convertPyDictToCppArray(pyValue));
        } else {
            // Handle unsupported types as needed.
            cppArray->value.push_back(nullptr);
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
    if (isAssociativeArray(cppValue)) {
        const auto& associativeArray = dynamic_cast<const AssociativeArray&>(*cppValue);
        PyObject* pyDict = PyDict_New();

        for (const auto& pair : associativeArray.value) {
            PyObject* pyKey = PyUnicode_DecodeFSDefault(pair.first.c_str());
            PyObject* pyValue = convertCppArrayToPyObject(pair.second);

            PyDict_SetItem(pyDict, pyKey, pyValue);

            Py_XDECREF(pyKey);
            Py_XDECREF(pyValue);
        }

        return pyDict;
    } else {
        const auto& indexedArray = dynamic_cast<const IndexedArray&>(*cppValue);
        PyObject* pyList = PyList_New(indexedArray.value.size());

        for (size_t i = 0; i < indexedArray.value.size(); ++i) {
            PyObject* pyValue = convertCppArrayToPyObject(indexedArray.value[i]);
            PyList_SET_ITEM(pyList, i, pyValue);
        }

        return pyList;
    }
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
