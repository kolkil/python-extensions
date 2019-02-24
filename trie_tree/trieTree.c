#ifdef DEBUG
#include <stdio.h>
#endif
#include <Python.h>
#include "structmember.h"
#include "tree/tree.h"

typedef struct
{
    PyObject_HEAD;
    tree *trie_tree;
} trieTreeObject;

static void trieTree_dealloc(trieTreeObject *self)
{
    free_tree(self->trie_tree);
}

static PyObject *trieTree_new(PyTypeObject *type)
{
    trieTreeObject *self = (trieTreeObject *)type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->trie_tree = get_tree();
#ifdef DEBUG
        printf("construct, %s\n", self->trie_tree == NULL ? "NULL!" : "NOT NULL!");
#endif
    }
    return (PyObject *)self;
}

static int trieTree_init(trieTreeObject *self)
{
    return 0;
}

static PyMemberDef trieTree_members[] = {
    {NULL}};

static PyObject *trieTree_insertWord(trieTreeObject *self, PyObject *word)
{
    char *tmp;
    if (!PyArg_ParseTuple(word, "s", &tmp))
        return NULL;
    int counter = insert_word(self->trie_tree, tmp);
#ifdef DEBUG
    printf("c = %d, tmp = %s, self->trie_tree = %s\n", counter, tmp, self->trie_tree == NULL ? "NULL" : "Tree");
#endif
    return Py_BuildValue("i", counter);
}

static PyMethodDef trieTree_methods[] = {
    {"insertWord", (PyCFunction)trieTree_insertWord, METH_VARARGS, "insert word to trie and return number"},
    {NULL}};

static PyTypeObject trieTreeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "trieTree",
    .tp_doc = "trieTree objects",
    .tp_basicsize = sizeof(trieTreeObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = trieTree_new,
    .tp_init = (initproc)trieTree_init,
    .tp_dealloc = (destructor)trieTree_dealloc,
    .tp_members = trieTree_members,
    .tp_methods = trieTree_methods,
};

static PyModuleDef trietreemodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "trieTree",
    .m_doc = "Trie tree module.",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_trieTree(void)
{
    PyObject *m;
    if (PyType_Ready(&trieTreeType) < 0)
        return NULL;

    m = PyModule_Create(&trietreemodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&trieTreeType);
    PyModule_AddObject(m, "trieTree", (PyObject *)&trieTreeType);
    return m;
}
