#include <vector>
#include <unordered_set>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

struct Cell {
    mutable size_t root_id;
    mutable size_t tree_size;
    bool open;
};

typedef pair<size_t, size_t> coord;

class Percolation {
    size_t side_len;
    vector<Cell> cells;
    mutable size_t number_of_open_sites;

    #define TOP_ROW 0
    #define BOTTOM_ROW side_len + 1

public:

    // creates n-by-n grid, with all sites initially blocked
    Percolation(size_t n) : side_len(n), cells(side_len * (side_len + 2)), number_of_open_sites(0) {

        for (size_t id = 0; id < cells.size(); ++id) {
            cells[id].root_id = id;
            cells[id].tree_size = 1;
            cells[id].open = false;
        }

        openRaw(getIdRaw(TOP_ROW, 0));
        openRaw(getIdRaw(BOTTOM_ROW, 0));

        for (size_t i = 1; i < side_len; ++i) {
            openRaw(getIdRaw(TOP_ROW, i));
            connect(getIdRaw(TOP_ROW, 0), getIdRaw(TOP_ROW, i));
            openRaw(getIdRaw(BOTTOM_ROW, i));
            connect(getIdRaw(BOTTOM_ROW, 0), getIdRaw(BOTTOM_ROW, i));
        }
    }

    inline size_t root(size_t id) const {
        if (cells[id].root_id == id) {
            return id;
        } else {
            return cells[id].root_id = root(cells[id].root_id);
        }

        // while (cells[id].root_id != id) {
        //     auto ts = cells[id].tree_size;
        //     id = cells[id].root_id = cells[cells[id].root_id].root_id;
        // }

        // return id;
    }

    inline void connectRaw(coord p_coord, coord q_coord) {
        connect(getIdRaw(p_coord), getIdRaw(q_coord));
    }

    inline void connect(coord p_coord, coord q_coord) {
        connect(getId(p_coord), getId(q_coord));
    }

    void connect(size_t lesser_tree_id, size_t greater_tree_id) {
        if (!isOpen(lesser_tree_id) || !isOpen(greater_tree_id)) {
            return;
        } else if (cells[root(lesser_tree_id)].tree_size > cells[root(greater_tree_id)].tree_size) {
            swap(lesser_tree_id, greater_tree_id);
        }

        cells[lesser_tree_id].root_id = root(greater_tree_id);
        cells[cells[lesser_tree_id].root_id].tree_size += cells[lesser_tree_id].tree_size;
    }

    bool connected(size_t p_id, size_t q_id) const {
        return root(p_id) == root(q_id);
    }

    inline size_t getId(size_t row, size_t col) const {
        return (1 + row) * side_len + col;
    }

    inline size_t getId(coord p) const {
        return getId(p.first, p.second);
    }

    inline size_t getIdRaw(size_t row, size_t col) const {
        return row * side_len + col;
    }

    inline size_t getIdRaw(coord p) const {
        return getIdRaw(p.first, p.second);
    }

    // opens the site (row, col) if it is not open already
    void openRaw(size_t id) {
        if (!isOpen(id)) {
            ++number_of_open_sites;
            cells[id].open = true;
        }
    }

    void open(coord c) {
        openRaw(getId(c));

        if (c.first - 1 >= TOP_ROW) 
            connect(c, {c.first - 1, c.second});
        if (c.first + 1 <= BOTTOM_ROW) 
            connect(c, {c.first + 1, c.second});
        if (c.second - 1 >= TOP_ROW)
            connect(c, {c.first, c.second - 1});
        if (c.second + 1 <= BOTTOM_ROW) 
            connect(c, {c.first, c.second + 1});
    }

    // is the site (row, col) open?
    bool isOpen(int row, int col) const {
        return isOpen(getId(row, col));
    }

    bool isOpen(size_t id) const {
        return cells[id].open;
    }

    // returns the number of open sites
    int numberOfOpenSites() const {
        return number_of_open_sites;
    }

    // does the system percolate?
    bool percolates() {
        // auto first = getIdRaw({TOP_ROW, 0});
        // auto almost_last = getIdRaw({BOTTOM_ROW, side_len - 2});
        // auto last = getIdRaw({BOTTOM_ROW, side_len - 1});
        return connected(getIdRaw({TOP_ROW, 0}), getIdRaw({BOTTOM_ROW, side_len - 1}));
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

    printSites(p);

    p.open({0, 0});
    p.open({0, 1});
    p.open({1, 1});
    p.open({1, 2});
    p.open({2, 1});

    printSites(p);

    cout << (p.percolates() ? "yes" : "no");

    return 0;
}