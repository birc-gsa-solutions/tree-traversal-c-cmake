#ifndef GSA_H
#define GSA_H

#include <stdlib.h>

// Dynamic array that grows as we append to it
typedef struct dynarr_data *dynarr;
struct dynarr_data
{
    size_t len;
    size_t cap;
    int data[];
};

dynarr new_dynarr(void);
void append(dynarr *a, int i);

typedef struct node *tree;
struct node
{
    int val;
    tree left, right;
};

tree new_tree(int val, tree left, tree right);
void free_tree(tree t);

dynarr in_order(tree t);
dynarr bf_order(tree t);

// hacking our way out of a stack
dynarr in_order2(tree t);

#endif // GSA_H
