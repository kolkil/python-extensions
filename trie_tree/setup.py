from distutils.core import setup, Extension
module = Extension('trieTree', sources=['trieTree.c', 'tree/tree.c'])
setup(name='trieTree', version='1.0', ext_modules=[module])
