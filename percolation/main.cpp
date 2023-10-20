#include <vector>
#include <unordered_set>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

struct Cell {
    mutable size_t root_id;
    size_t tree_size;
    bool open;
};

class Percolation {
    size_t side_len;
    vector<Cell> cells;
    unordered_set<size_t> open_roots;

    #define TOP_CELL 0
    #define BOTTOM_CELL side_len * (side_len + 1)

public:

    // creates n-by-n grid, with all sites initially blocked
    Percolation(size_t n) : side_len(n), cells(side_len * (side_len + 2)) {

        for (size_t id = 0; id < cells.size(); ++id) {
            cells[id].root_id = id;
            cells[id].tree_size = 1;
            cells[id].open = false;
        }

        for (size_t id = 0; id < side_len; ++id) {
            open(TOP_CELL + id, TOP_CELL);
            connect(BOTTOM_CELL + id, BOTTOM_CELL);
        }
    }

    size_t root(size_t id) const {
        if (cells[id].root_id == id) {
            return id;
        } else {
            return cells[id].root_id  = root(cells[id].root_id);
        }
    }

    void connect(size_t p_id, size_t q_id) {
        if (!isOpen(root(p_id)) || !isOpen(root(q_id))) {
            return;
        }
        else if (cells[p_id].tree_size <= cells[q_id].tree_size) {
            cells[p_id].root_id = root(q_id);
            ++cells[p_id].tree_size;
        } else {
            connect(q_id, p_id);
        }
    }

    bool connected(size_t p_id, size_t q_id) const {
        return root(p_id) == root(q_id);
    }

    inline size_t getId(size_t row, size_t col) const {
        return (1 + row) * side_len + col;
    }

    // opens the site (row, col) if it is not open already
    void open(size_t id) {
        open_roots.insert(root(id));

        if (row - 1 >= 0) {
            connect(getId(row, col), getId(row - 1, col));
        }
        if (col - 1 >= 0) {
            connect(getId(row, col), getId(row, col - 1));
        }
        if (col + 1 < side_len) {
            connect(getId(row, col), getId(row, col + 1));
        }
        if (row + 1 < side_len + 1) {
            connect(getId(row, col), getId(row + 1, col));
        }
    }

    void open(size_t row, size_t col) {
    }

    // is the site (row, col) open?
    bool isOpen(int row, int col) const {
        return isOpen(getId(row, col));
    }

    bool isOpen(size_t id) const {
        return open_roots.count(root(id)) > 0;
    }

    // is the site (row, col) full?
    // bool isFull(int row, int col) {
    //     return ;
    // }

    // returns the number of open sites
    int numberOfOpenSites() const {
        size_t result = 0;
        
        for (auto& id: open_roots) {
            result += cells[id].tree_size;
        }

        return result;
    }

    // does the system percolate?
    bool percolates() const {
        return connected(BOTTOM_CELL, TOP_CELL);
    }

    vector<string> print() const {
        vector<string> result(1 + side_len + 1);

        result[0] = string(side_len, '+');
        result[side_len + 1] = result[0];

        for (size_t i = 0; i < side_len; ++i) {
            string& row = result[i + 1];

            for (size_t j = 0; j < side_len; ++j) {
                row.push_back(isOpen(i, j) ? '+' : '-');
            }
        }

        return result;
    }
};

void printSites(const Percolation& p) {
    for (const auto& s: p.print()) {
        cout << s << '\n';
    }
    cout << '\n';
}

int main(int argc, char* argv[]) {
    Percolation p(3);

    //printSites(p);

    p.open(0, 1);
    p.open(1, 1);
    p.open(2, 1);

    //printSites(p);

    cout << (p.percolates() ? "yes" : "no";

    return 0;
}