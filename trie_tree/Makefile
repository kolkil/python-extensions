CC = gcc
CFLAGS = -Wall -Wextra -O3
LIB_DIR = tree_lib_dir

all: main clean

debug: CFLAGS += -DDEBUG
debug: all

main: clean_build
	sudo python3 setup.py install

tree.o: 
	$(CC) $(CFLAGS) -Werror -fpic -c tree/tree.c tree/tree.h

tree.so:
	$(CC) $(CFLAGS) -shared -o tree_lib_dir/tree.so tree.o

tree.dir:
	mkdir -p $(LIB_DIR)

tree.rm:
	rm -rf $(LIB_DIR)/tree.so

clean_build:
	sudo rm -rf build

clean:
	rm -f *.o; rm -f *.gch; rm -f ./*/*.gch;