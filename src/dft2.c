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

// Move child <- parent and parent <- grandparent
// and restore the tagged child
static void go_up(tree *parent, tree *child)
{
    tree c = *child, p = *parent, grandparent = 0;
    if (is_tagged(p->left))
    {
        grandparent = untag_pointer(p->left);
        p->left = c;
    }
    else
    {
        grandparent = untag_pointer(p->right);
        p->right = c;
    }
    *child = p;
    *parent = grandparent;
}

dynarr in_order2(tree t)
{
    dynarr a = new_dynarr();

    // Using a dummy root to avoid special cases with a NULL parent
    struct node dummy_root = {.val = 0, .left = t, .right = 0};
    tree parent = &dummy_root;
    bool moving_left = true;

    while (t && t != &dummy_root)
    {
        if (moving_left)
        {
            if (t->left)
            { // Move left when we can
                go_left(&t, &parent);
                continue;
            }
            else
            { // Otherwise, change direction
                moving_left = false;
            }
        }

        // If we get here, we emit and move right or up
        append(&a, t->val);

        if (t->right) // Try to move right
        {
            go_right(&t, &parent);
            moving_left = true;
        }
        else
        {
            // Run up the right-pointing-path and take one left step up
            while (is_tagged(parent->right))
                go_up(&parent, &t);
            go_up(&parent, &t);
        }
    }

    return a;
}
