#ifdef DEBUG
#include <stdio.h>
#endif
#include <string.h>
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

static PyObject *trieTree_insert(trieTreeObject *self, PyObject *word)
{
    char *tmp;
    if (!PyArg_ParseTuple(word, "s", &tmp))
        return Py_BuildValue("i", 0);
    int counter = insert_word(self->trie_tree, tmp);
#ifdef DEBUG
    printf("c = %d, tmp = %s, self->trie_tree = %s\n", counter, tmp, self->trie_tree == NULL ? "NULL" : "Tree");
#endif
    return Py_BuildValue("i", counter);
}

static PyObject *trieTree_insertWithValue(trieTreeObject *self, PyObject *args)
{
    char *tmp;
    int value;
    if (!PyArg_ParseTuple(args, "si", &tmp, &value))
        return Py_BuildValue("i", 0);
    int counter = insert_word_with_value(self->trie_tree, tmp, value);
    return Py_BuildValue("i", counter);
}

static PyObject *trieTree_inTree(trieTreeObject *self, PyObject *args)
{
    char *word;
    if (!PyArg_ParseTuple(args, "s", &word))
        return Py_BuildValue("i", 0);
    int counter = in_tree(self->trie_tree, word);
    return Py_BuildValue("i", counter);
}

static PyObject *trieTree_printTree(trieTreeObject *self, PyObject *args)
{
    print_tree_words_count(self->trie_tree);
    return Py_BuildValue("i", 0);
}

static PyObject *trieTree_treeToList(trieTreeObject *self, PyObject *args)
{
    item *array = malloc(sizeof(item) * self->trie_tree->words);
    tree_to_array(self->trie_tree, array);
    int len = self->trie_tree->words;
    PyObject *list = PyList_New(len);
    for (int i = 0; i < len; ++i)
    {
        PyObject *pair = Py_BuildValue("si", array[i].word, array[i].count);
        PyList_SetItem(list, i, pair);
        free(array[i].word);
    }
    free(array);
    return list;
}

static PyObject *trieTree_countAllWithPrefix(trieTreeObject *self, PyObject *args)
{
    char *word;
    if (!PyArg_ParseTuple(args, "s", &word))
        return Py_BuildValue("i", 0);
    return Py_BuildValue("i", count_all_with_prefix(self->trie_tree, word));
}

static PyMethodDef trieTree_methods[] = {
    {"insert", (PyCFunction)trieTree_insert, METH_VARARGS, "insert word to trie and return number"},
    {"insertWithValue", (PyCFunction)trieTree_insertWithValue, METH_VARARGS, "insert word to trie with value and return number"},
    {"inTree", (PyCFunction)trieTree_inTree, METH_VARARGS, "returns number of occurrences"},
    {"printTree", (PyCFunction)trieTree_printTree, METH_NOARGS, "prints trie tree"},
    {"treeToList", (PyCFunction)trieTree_treeToList, METH_NOARGS, "returns list of tree content"},
    {"countAllWithPrefix", (PyCFunction)trieTree_countAllWithPrefix, METH_VARARGS, "returns number of words with specified prefix"},
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
