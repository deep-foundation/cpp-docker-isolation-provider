#ifndef CPP_DOCKER_ISOLATION_PROVIDER_PYCPPBRIDGE_H
#define CPP_DOCKER_ISOLATION_PROVIDER_PYCPPBRIDGE_H


class PyCppBridge {
public:
    static bool isAssociativeArray(const Php::Value& phpArray);
    static Php::Value convertPyDictToPhpArray(PyObject* pyDict);
    static Php::Value convertPyListToPhpArray(PyObject* pyList);
    static PyObject* convertPhpArrayToPyDict(const Php::Value& phpArray);
    static PyObject* convertPhpArrayToPyList(const Php::Value& phpArray);
    static PyObject* convertPhpValueToPyObject(const Php::Value& phpValue);
    static std::string getPythonErrorText();
};


#endif //CPP_DOCKER_ISOLATION_PROVIDER_PYCPPBRIDGE_H
