#include "gsa.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

/*
 This is an attempt at using tagged pointers to traverse the tree.
 I haven't tested it super carefully, but it should illustrate the
 underlying idea.
 */

// Tagged pointers to store parent in left or right while
// remembering which child holds the parent
static inline tree tag_pointer(tree t)
{
    return (tree)((uintptr_t)t | (uintptr_t)0x1);
}

static inline tree untag_pointer(tree t)
{
    return (tree)((uintptr_t)t & ~(uintptr_t)0x1);
}

static inline bool is_tagged(tree t)
{
    return ((uintptr_t)t) & (uintptr_t)0x1;
}

// Move to the left child, setting a tagged pointer
// in the new parent.
static void go_left(tree *child, tree *parent)
{
    tree left = (*child)->left;
    (*child)->left = tag_pointer(*parent);
    *parent = *child;
    *child = left;
}

// Move to the right child, setting a tagged pointer
// in the new parent.
static void go_right(tree *child, tree *parent)
{
    tree right = (*child)->right;
    (*child)->right = tag_pointer(*parent);
    *parent = *child;
    *child = right;
}

// If t is the left child of p, move us to p
static void go_up_as_left_child(tree *t, tree *p)
{
    assert(*p && is_tagged((*p)->left));
    tree grandparent = untag_pointer((*p)->left);
    (*p)->left = *t;
    *t = *p;
    *p = grandparent;
}

// If t is the right child of p, move us to p
static void go_up_as_right_child(tree *t, tree *p)
{
    assert(*p && is_tagged((*p)->right));
    tree grandparent = untag_pointer((*p)->right);
    (*p)->right = *t;
    *t = *p;
    *p = grandparent;
}

// Run all the way to the left from t
static void go_to_leftmost(tree *t, tree *p)
{
    while (*t && (*t)->left)
        go_left(t, p);
}

// Go upwards until we are the left child of a parent,
// then stop at that parent
static void go_to_next_emitable_parent(tree *t, tree *p)
{
    tree grandparent = 0;

    // up to parent we are right child of
    while (*p && is_tagged((*p)->right))
        go_up_as_right_child(t, p);
    
    // up one more (left) parent
    go_up_as_left_child(t, p);
}

dynarr in_order2(tree t)
{
    dynarr a = new_dynarr();

    // Using a dummy root to avoid special cases with a NULL parent
    struct node dummy_root = {.val = 0, .left = tag_pointer(0), .right = 0};
    tree parent = &dummy_root;
    bool moving_left = false;

    go_to_leftmost(&t, &parent); // Start in the leftmost node
    while (t && t != &dummy_root)
    {
        // Emit
        append(&a, t->val);

        if (t->right) // If we can go right, we do that and go all left
        {
            go_right(&t, &parent);
            go_to_leftmost(&t, &parent);
        }
        else // Run up the right-going-path and take one step as a left child
        {
            go_to_next_emitable_parent(&t, &parent);
        }
    }

    return a;
}
