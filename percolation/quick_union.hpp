#include <vector>

class QuickUnion {
    std::vector<int> id;
    std::vector<int> sz;
public:
    inline QuickUnion(std::size_t N) : id(N), sz(N, 1) {
        for (int i = 0; i < N; ++i) {
            id[i] = i;
        }
    }

    inline int root(int p) {
        while (p != id[p]) {
            id[p] = id[id[p]];
            p = id[p];
        }
        return p;
    }

    inline bool connected(int p, int q) {
        return root(id[p]) == root(id[q]);
    }

    inline void unite(int p, int q) {
        auto p_root = root(p);
        auto q_root = root(q);

        if (sz[q_root] > sz[p_root]) {
            id[p_root] = q_root;
            ++sz[p_root];
        }
        else {
            id[q_root] = p_root;
            ++sz[q_root];
        }
    }
};