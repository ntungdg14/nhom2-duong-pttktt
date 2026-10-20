// cht_li_chao.cpp
// Li Chao segment tree: fully dynamic CHT with arbitrary slope order.
// When slope constraints are not monotone, use Li Chao tree for O(log X) per op.
//
// Node stores the best (currently winning) line over its segment; query goes
// down the tree and decides which child to recurse into.

#include <vector>
#include <cstdint>
#include <limits>
#include <iostream>
#include <algorithm>
#include <cassert>

using i64 = long long;
static const i64 INF64 = (1LL << 62);

struct Line {
    i64 m = 0, b = 0;
    bool empty = true;

    Line() = default;
    Line(i64 m_, i64 b_) : m(m_), b(b_), empty(false) {}

    i64 get(i64 x) const {
        // This example assumes all real m*x+b values fit in long long.
        return empty ? INF64 : m * x + b;
    }
};

class LiChao {
    i64 X_L, X_R;
    std::vector<Line> seg;

    void add_line(Line nw, int node, i64 l, i64 r) {
        i64 mid = l + (r - l) / 2;
        if (seg[node].empty) {
            seg[node] = nw;
            return;
        }

        // Keep the line that is better at mid in this node.
        if (nw.get(mid) < seg[node].get(mid))
            std::swap(seg[node], nw);
        if (l == r) return;

        // The remaining line can win on at most one side.
        if (nw.get(l) < seg[node].get(l))
            add_line(nw, node << 1, l, mid);
        else if (nw.get(r) < seg[node].get(r))
            add_line(nw, node << 1 | 1, mid + 1, r);
    }

    i64 query(i64 x, int node, i64 l, i64 r) const {
        i64 ans = seg[node].get(x);
        if (l == r) return ans;
        i64 mid = l + (r - l) / 2;
        if (x <= mid)
            return std::min(ans, query(x, node << 1, l, mid));
        return std::min(ans, query(x, node << 1 | 1, mid + 1, r));
    }

public:
    LiChao(i64 lo, i64 hi) : X_L(lo), X_R(hi),
        seg(4 * static_cast<size_t>(hi - lo + 1) + 10) {
        assert(lo <= hi);
    }

    void add_line(i64 m, i64 b) {
        add_line(Line(m, b), 1, X_L, X_R);
    }

    i64 query(i64 x) const {
        assert(X_L <= x && x <= X_R);
        return query(x, 1, X_L, X_R);
    }
};

int main() {
    LiChao tree(0, (i64)1e6);
    tree.add_line(-1, 0);
    tree.add_line(-4, 50);
    tree.add_line(-2, 12);
    std::cout << "Query x=10: " << tree.query(10) << "\n";
    return 0;
}
