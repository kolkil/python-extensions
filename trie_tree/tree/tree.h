#include <stdint.h>

#define ALPHABET_LEN 26
#define STANDARD_BUFFER_SIZE 1024

typedef struct dict_node dict_node;
typedef struct dict_node
{
    uint32_t ends;
    dict_node *children[ALPHABET_LEN];
} dict_node;

typedef struct tree tree;
typedef struct tree
{
    dict_node *roots[ALPHABET_LEN];
    uint32_t nodes,
        words;
} tree;

typedef struct
{
    char *word;
    uint32_t count;
} item;

int insert_word(tree *, char[STANDARD_BUFFER_SIZE]);
int insert_word_with_value(tree *, char[STANDARD_BUFFER_SIZE], uint32_t);
int print_tree_words_count(tree *);
int in_tree(tree *, char[STANDARD_BUFFER_SIZE]);
tree *get_tree();
int free_tree(tree *);
void tree_to_array(tree *, item *);
