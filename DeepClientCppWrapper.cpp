#include "compiler.h"


class DeepClientCppWrapper {

private:
    std::string token;
    std::string url;
    PyObject* deepClientModule = nullptr;

    void initializePython() {
        Py_Initialize();
        PyRun_SimpleString("import sys\n"
                           "import os\n"
                           "print(\"cwd:\", os.getcwd())\n"
                           "print(\"Python:\", sys.version)\n"
                           "sys.path.append('.');");
        deepClientModule = PyImport_ImportModule("deep_client_interface");
        if (!deepClientModule) {
            PyErr_Print();
            throw std::runtime_error("Failed to import required Python modules");
        }
    }

    void finalizePython() {
        Py_XDECREF(deepClientModule);
        Py_Finalize();
    }

public:
    DeepClientCppWrapper() {
        initializePython();
    }

    ~DeepClientCppWrapper() {
        finalizePython();
    }

    DeepClientCppWrapper(const std::string& jwt, const std::string& gql_urn_str) {
        initializePython();
        token = jwt;
        url = gql_urn_str;
    }

    std::shared_ptr<DynamicValue> select(std::shared_ptr<DynamicValue> query) {
        return call_python_function("select", query);
    }

    std::shared_ptr<DynamicValue> insert(std::shared_ptr<DynamicValue> query) {
        return call_python_function("insert", query);
    }

    std::shared_ptr<DynamicValue> update(std::shared_ptr<DynamicValue> query) {
        return call_python_function("update", query);
    }

    std::shared_ptr<DynamicValue> deleteFunc(std::shared_ptr<DynamicValue> query) {
        return call_python_function("delete", query);
    }

    std::shared_ptr<DynamicValue> serial(std::shared_ptr<DynamicValue> query) {
        return call_python_function("serial", query);
    }

    std::shared_ptr<DynamicValue> id(std::shared_ptr<DynamicValue> query) {
        return call_python_function("id", query);
    }

    std::shared_ptr<DynamicValue> call_python_function(const std::string& function_name, std::shared_ptr<DynamicValue>& query) {
        std::shared_ptr<DynamicValue> result;
        if (deepClientModule) {
            PyObject* pyFunc = PyObject_GetAttrString(deepClientModule, function_name.c_str());
            if (pyFunc && PyCallable_Check(pyFunc)) {
                PyObject* pyArgs = PyTuple_Pack(3,
                                                Py_BuildValue("s", token.c_str()),
                                                Py_BuildValue("s", url.c_str()),
                                                PyCppBridge::convertCppArrayToPyObject(query)
                );
                PyObject* pyResult = PyObject_CallObject(pyFunc, pyArgs);
                if (pyResult) {
                    if (PyUnicode_Check(pyResult)) {
                        const char* str_value = PyUnicode_AsUTF8(pyResult);
                        throw std::runtime_error(str_value);
                    } else if (PyList_Check(pyResult)) {
                        // std::cout << "This is an IndexedArray" << std::endl;
                        return PyCppBridge::convertPyListToCppArray(pyResult);
                    } else if (PyDict_Check(pyResult)) {
                        // std::cout << "This is an AssociativeArray" << std::endl;
                        return PyCppBridge::convertPyDictToCppArray(pyResult);
                    } else {
                        std::string errorText = PyCppBridge::getPythonErrorText();
                        throw std::runtime_error(errorText.c_str());
                    }
                    Py_DECREF(pyResult);
                } else {
                    PyErr_Print();
                    throw std::runtime_error(PyCppBridge::getPythonErrorText());
                }
                Py_DECREF(pyArgs);
            } else {
                PyErr_Print();
                throw std::runtime_error("Failed to load required Python modules");
            }
            Py_XDECREF(pyFunc);
        } else {
            throw std::runtime_error("Failed to import required Python modules in call_python_function");
        }
        return result;
    }
};
