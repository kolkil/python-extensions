#include "tree.h"

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

char downer_case(char c)
{
    if (c > 96 && c < 123)
        return c;
    if (c > 64 && c < 91)
        return c + 'a' - 'A';
    return 'a';
}

short is_letter(char c)
{
    if (c > 96 && c < 123)
        return 1;
    if (c > 64 && c < 91)
        return 1;
    return 0;
}

tree *get_tree()
{
    tree *base = malloc(sizeof(tree));
    for (int i = 0; i < ALPHABET_LEN; ++i)
        base->roots[i] = NULL;
    base->nodes = 0;
    base->words = 0;
    return base;
}

dict_node *get_node()
{
    dict_node *node = malloc(sizeof(dict_node));
    for (int i = 0; i < ALPHABET_LEN; ++i)
        node->children[i] = NULL;
    node->ends = 0;
    return node;
}

void match_letters(char input[STANDARD_BUFFER_SIZE], char output[STANDARD_BUFFER_SIZE])
{
    uint32_t len = strlen(input);
    for (uint32_t i = 0, k = 0; i < len; ++i)
    {
        if (input[i] > 0)
        {
            if (!is_letter(input[i]))
            {
                ++i;
                continue;
            }
            output[k++] = downer_case(input[i]);
        }
        else if (input[i] == -60)
        {
            if (i == len - 1)
                continue;
            ++i;
            if (input[i] == -123)
                output[k++] = 'a';
            else if (input[i] == -124)
                output[k++] = 'a';
            else if (input[i] == -121)
                output[k++] = 'c';
            else if (input[i] == -122)
                output[k++] = 'c';
            else if (input[i] == -103)
                output[k++] = 'e';
            else if (input[i] == -104)
                output[k++] = 'e';
        }
        else if (input[i] == -59)
        {
            if (i == len - 1)
                continue;
            ++i;
            if (input[i] == -126)
                output[k++] = 'l';
            else if (input[i] == -127)
                output[k++] = 'l';
            else if (input[i] == -69)
                output[k++] = 'z';
            else if (input[i] == -68)
                output[k++] = 'z';
            else if (input[i] == -70)
                output[k++] = 'z';
            else if (input[i] == -71)
                output[k++] = 'z';
            else if (input[i] == -101)
                output[k++] = 's';
            else if (input[i] == -102)
                output[k++] = 's';
        }
        else if (input[i] == -61)
        {
            if (i == len - 1)
                continue;
            ++i;
            if (input[i] == -77)
                output[k++] = 'o';
            else if (input[i] == -109)
                output[k++] = 'o';
        }
    }
    output[STANDARD_BUFFER_SIZE - 1] = 0;
}

int insert_word(tree *base, char input[STANDARD_BUFFER_SIZE])
{
    char word[STANDARD_BUFFER_SIZE] = {0};
    match_letters(input, word);
    uint32_t len = strlen(word);
    if (len <= 0)
        return 0;
    int index = (int)word[0] - 'a';
    if (base->roots[index] == NULL)
    {
        base->roots[index] = get_node();
        ++base->nodes;
    }
    dict_node *current = base->roots[index];
    for (uint32_t i = 0; i < len; ++i)
    {
        if (i > 0)
        {
            index = (int)word[i] - 'a';
            if (current->children[index] == NULL)
            {
                current->children[index] = get_node();
                current->children[index]->ends = 0;
                ++base->nodes;
            }
            current = current->children[index];
        }
    }
    ++current->ends;
    if (current->ends == 1)
        ++base->words;
    return current->ends - 1;
}

int insert_word_with_value(tree *base, char input[STANDARD_BUFFER_SIZE], uint32_t count)
{
    char word[STANDARD_BUFFER_SIZE] = {0};
    match_letters(input, word);
    uint32_t len = strlen(word);
    if (len <= 0)
        return 0;
    int index = (int)word[0] - 'a';
    if (base->roots[index] == NULL)
    {
        base->roots[index] = get_node();
        ++base->nodes;
    }
    dict_node *current = base->roots[index];
    for (uint32_t i = 0; i < len; ++i)
    {
        if (i > 0)
        {
            index = (int)word[i] - 'a';
            if (current->children[index] == NULL)
            {
                current->children[index] = get_node();
                current->children[index]->ends = 0;
                ++base->nodes;
            }
            current = current->children[index];
        }
    }
    if (current->ends == 0)
        ++base->words;
    current->ends = count;
    return current->ends;
}

int in_tree(tree *base, char input[STANDARD_BUFFER_SIZE])
{
    char word[STANDARD_BUFFER_SIZE] = {0};
    match_letters(input, word);
    uint32_t len = strlen(word);
    if (len <= 0)
        return 0;
    int index = (int)word[0] - 'a';
    if (base->roots[index] == NULL)
        return 0;
    dict_node *current = base->roots[index];
    for (uint32_t i = 1; i < len; ++i)
    {
        index = (int)word[i] - 'a';
        if (current->children[index] == NULL)
            return 0;
        current = current->children[index];
    }
    return current->ends;
}

void print_node_children_c(dict_node *node, int offset, int *w_f)
{
    if (node == NULL)
    {
        printf("\n");
        return;
    }
    int not_nulls = 0;
    for (int i = 0, n_printed = 0; i < ALPHABET_LEN; ++i)
    {
        if (node->children[i] != NULL)
        {
            if (*w_f || node->ends > 0)
            {
                if (node->ends > 0 && !n_printed)
                {
                    printf("\t%d\n", node->ends);
                    n_printed = 1;
                }
                for (int k = 0; k < offset; ++k)
                    printf(" ");
                *w_f = 0;
            }
            ++not_nulls;
            printf("%c", (char)('a' + i));
            print_node_children_c(node->children[i], offset + 1, w_f);
        }
    }
    if (not_nulls == 0)
    {
        *w_f = 1;
        printf("\t%d\n", node->ends);
    }
}

int print_tree_count(tree *base)
{
    for (int i = 0; i < ALPHABET_LEN; ++i)
        if (base->roots[i] != NULL)
        {
            printf("%c", (char)('a' + i));
            int w_f = 0;
            print_node_children_c(base->roots[i], 1, &w_f);
            if (i < ALPHABET_LEN - 1)
                printf("\n");
        }
    return 0;
}

void print_node_children_c_w(dict_node *node, char offset[STANDARD_BUFFER_SIZE], int *w_f)
{
    if (node == NULL)
    {
        printf("\n");
        return;
    }
    int not_nulls = 0;
    for (int i = 0, n_printed = 0; i < ALPHABET_LEN; ++i)
    {
        if (node->children[i] != NULL)
        {
            if (*w_f || node->ends > 0)
            {
                if (node->ends > 0 && !n_printed)
                {
                    printf("\t%d\n", node->ends);
                    n_printed = 1;
                }
                printf("%s", offset);
                *w_f = 0;
            }
            ++not_nulls;
            printf("%c", (char)('a' + i));
            int len = strlen(offset);
            if (len < STANDARD_BUFFER_SIZE - 1)
                offset[len] = (char)('a' + i);
            print_node_children_c_w(node->children[i], offset, w_f);
            if (len < STANDARD_BUFFER_SIZE - 1)
                offset[len] = 0;
        }
    }
    if (not_nulls == 0)
    {
        *w_f = 1;
        int len = strlen(offset);
        offset[len - 1] = 0;
        printf("\t%d\n", node->ends);
    }
}

int print_tree_words_count(tree *base)
{
    char offset[STANDARD_BUFFER_SIZE] = {0};
    for (int i = 0; i < ALPHABET_LEN; ++i)
        if (base->roots[i] != NULL)
        {
            printf("%c", (char)('a' + i));
            int w_f = 0;
            offset[0] = (char)('a' + i);
            print_node_children_c_w(base->roots[i], offset, &w_f);
            if (i < ALPHABET_LEN - 1)
                printf("\n");
        }
    return 0;
}

void free_node_children(dict_node *node, int *frees)
{
    if (node == NULL)
    {
        return;
    }
    int not_nulls = 0;
    for (int i = 0; i < ALPHABET_LEN; ++i)
    {
        if (node->children[i] != NULL)
        {
            ++not_nulls;
            free_node_children(node->children[i], frees);
        }
    }
    ++(*frees);
    free(node);
}

int free_tree(tree *base)
{
    int frees = 0;
    for (int i = 0; i < ALPHABET_LEN; ++i)
    {
        if (base->roots[i] != NULL)
        {
            free_node_children(base->roots[i], &frees);
        }
    }
    free(base);
    return frees;
}
