#include <Python.h>

static PyObject *sayHello(PyObject *self)
{
    return Py_BuildValue("s", "This is first extension, Hello!!:D");
}

static char sayHello_docs[] = "sayHello(), my first extension function!";

static PyMethodDef sayHello_methods[] = {
    {"sayHello", (PyCFunction)sayHello, METH_NOARGS, sayHello_docs},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef sayHello_definition = {
    PyModuleDef_HEAD_INIT,
    "sayHello",
    "Module ther returns string.",
    -1,
    sayHello_methods};

PyMODINIT_FUNC PyInit_sayHello(void)
{
    Py_Initialize();
    return PyModule_Create(&sayHello_definition);
}
