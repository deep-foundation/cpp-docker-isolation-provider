#include "compiler.h"

std::shared_ptr<AssociativeArray> PyCppBridge::convertPyDictToCppArray(PyObject *pyDict) {
    auto cppArray = std::make_shared<AssociativeArray>();

    PyObject* pyKey;
    PyObject* pyValue;
    Py_ssize_t pos = 0;

    while (PyDict_Next(pyDict, &pos, &pyKey, &pyValue)) {
        std::string key(PyUnicode_AsUTF8(pyKey));

        std::cout << "This is a key: " << key << std::endl;

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

std::shared_ptr<IndexedArray> PyCppBridge::convertPyListToCppArray(PyObject *pyList) {
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

PyObject *PyCppBridge::convertCppArrayToPyDict(const std::shared_ptr<AssociativeArray> &cppArray) {
    const auto& associativeArray = dynamic_cast<const AssociativeArray&>(*cppArray);
    PyObject* pyDict = PyDict_New();

    for (const auto& pair : associativeArray.value) {
        PyObject* pyKey = PyUnicode_DecodeFSDefault(pair.first.c_str());
        PyObject* pyValue = PyCppBridge::convertCppArrayToPyObject(pair.second);

        PyDict_SetItem(pyDict, pyKey, pyValue);

        Py_XDECREF(pyKey);
        Py_XDECREF(pyValue);
    }

    return pyDict;
}

PyObject *PyCppBridge::convertCppArrayToPyList(const std::shared_ptr<IndexedArray> &cppArray) {
    const auto& indexedArray = dynamic_cast<const IndexedArray&>(*cppArray);
    PyObject* pyList = PyList_New(indexedArray.value.size());

    for (size_t i = 0; i < indexedArray.value.size(); ++i) {
        PyObject* pyValue = PyCppBridge::convertCppArrayToPyObject(indexedArray.value[i]);
        PyList_SET_ITEM(pyList, i, pyValue);
    }

    return pyList;
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

PyObject *PyCppBridge::convertCppArrayToPyObject(const std::shared_ptr<DynamicValue> &sharedPtr) {
    if (std::shared_ptr<AssociativeArray> associativeArray = std::dynamic_pointer_cast<AssociativeArray>(sharedPtr)) {
        return PyCppBridge::convertCppArrayToPyDict(associativeArray);
    } else if (std::shared_ptr<IndexedArray> indexedArray = std::dynamic_pointer_cast<IndexedArray>(sharedPtr)) {
        return PyCppBridge::convertCppArrayToPyList(indexedArray);
    } else if (std::shared_ptr<StringValue> stringValue = std::dynamic_pointer_cast<StringValue>(sharedPtr)) {
        std::cout << "This is a StringValue with value: " << stringValue->value << std::endl;
        return PyUnicode_DecodeUTF8(stringValue->value.c_str(), stringValue->value.size(), nullptr);
    } else if (std::shared_ptr<ArrayValue> arrayValue = std::dynamic_pointer_cast<ArrayValue>(sharedPtr)) {
        std::cout << "This is an ArrayValue" << std::endl;
        Py_INCREF(Py_None);
        return Py_None;
    } else if (std::shared_ptr<IntValue> intValue = std::dynamic_pointer_cast<IntValue>(sharedPtr)) {
        std::cout << "This is an IntValue with value: " << intValue->value << std::endl;
        return PyLong_FromLong(intValue->value);
    } else if (std::shared_ptr<FloatValue> floatValue = std::dynamic_pointer_cast<FloatValue>(sharedPtr)) {
        std::cout << "This is a FloatValue with value: " << floatValue->value << std::endl;
        return PyFloat_FromDouble(floatValue->value);
    } else {
        std::cout << "Unknown type" << std::endl;
        Py_INCREF(Py_None);
        return Py_None;
    }
}
