#include <cassert>
#include "../quick_union.hpp"

int main() {
    QuickUnion q(7);
    int p = 3; // example p value

    q.unite(0, 3);
    q.unite(3, 6);

    assert(q.root(0) == 0);
    assert(q.root(3) == 0);
    assert(q.root(6) == 0);

    assert(!q.connected(4, 5));
    assert(q.connected(0, 0));
    assert(q.connected(3, 3));
    assert(q.connected(3, 6));

    return 0;
}