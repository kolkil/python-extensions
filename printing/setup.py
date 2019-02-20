from distutils.core import setup, Extension
setup(name='sayHello', version='1.0', ext_modules=[Extension('sayHello', ['sayHello.c'])])
