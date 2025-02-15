#include "gsa.h"
#include <assert.h>
#include <stdio.h>

typedef dynarr (*traversal)(tree);
static void test_expected(traversal trav, tree t,
                          size_t len, int expected[len])
{
    dynarr a = trav(t);
    assert(a->len == len);
    for (size_t i = 0; i < len; i++)
    {
        assert(expected[i] == a->data[i]);
    }
    free(a);
}

int main(void)
{
    // Add more tests as needed.

    tree t = new_tree(
        2,
        new_tree(1, NULL, NULL),
        new_tree(4, new_tree(3, NULL, NULL), new_tree(5, NULL, NULL)));
    test_expected(in_order, t, 5, (int[]){1, 2, 3, 4, 5});
    test_expected(in_order2, t, 5, (int[]){1, 2, 3, 4, 5});
    test_expected(bf_order, t, 5, (int[]){2, 1, 4, 3, 5});
    free(t);

    // 0 in expected is ignored but required by standard
    test_expected(in_order, NULL, 0, (int[]){0});
    test_expected(in_order2, NULL, 0, (int[]){0});
    test_expected(bf_order, NULL, 0, (int[]){0});

    return 0;
}
