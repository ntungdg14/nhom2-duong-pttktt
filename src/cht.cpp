// cht.cpp
// Convex Hull Trick (deque-based, monotone slopes and queries)
//
// When the recurrence has the form
//   DP(i,j) = (m_j) * x_i + b_j + const(i)
// and slopes m_j are monotone (e.g. decreasing), the minimum of an upper
// envelope of straight lines can be answered in O(1) amortized using a deque
// that pops obsolete lines on the left and right.
//
// Implemented for dynamic insertion and amortized O(1) point-min queries.
// Each line y = m * x + b.

#include <deque>
#include <vector>
#include <cstdint>
#include <limits>
#include <iostream>

using i64 = long long;

struct Line {
    i64 m, b;
    Line(i64 m_, i64 b_) : m(m_), b(b_) {}
    i64 eval(i64 x) const { return m * x + b; }
};

// Returns true iff line l3 is worse than l2 when l2 follows l1
// (intersection of l1,l2 lies >= intersection of l2,l3).
bool bad(const Line& l1, const Line& l2, const Line& l3) {
    // (b3 - b1) * (m1 - m2) <= (b2 - b1) * (m1 - m3)   for integer-safety.
    __int128 left  = (__int128)(l3.b - l1.b) * (l1.m - l2.m);
    __int128 right = (__int128)(l2.b - l1.b) * (l1.m - l3.m);
    return left <= right;
}

class ConvexHullTrick {
    std::deque<Line> dq;
    bool min_query;
public:
    explicit ConvexHullTrick(bool minimize = true) : min_query(minimize) {}

    void add_line(i64 m, i64 b) {
        if (!min_query) { m = -m; b = -b; }
        Line nl(m, b);
        while (dq.size() >= 2 && bad(dq[dq.size()-2], dq[dq.size()-1], nl))
            dq.pop_back();
        dq.push_back(nl);
    }

    i64 query(i64 x) const {
        if (dq.empty()) {
            return std::numeric_limits<i64>::max();
        }
        // For monotone x queries: while x is at/past intersection of front two,
        // pop front.
        // (kept simple here: just pop while the second line is better)
        std::deque<Line>& q = const_cast<std::deque<Line>&>(dq);
        while (q.size() >= 2 && q[0].eval(x) >= q[1].eval(x))
            q.pop_front();
        i64 v = q[0].eval(x);
        return min_query ? v : -v;
    }
};

// Toy demo: solve DP(i,j) = min over k < i of { DP(k,j-1) + m_coef[i] * slope[k] + b[k] }
// where slope[k] = k and m_coef[i] = -i (line slopes decreasing). Not used in the
// baseline DP, but useful as a teaching example referenced in the report.
int main() {
    ConvexHullTrick cht(true);
    // Insert lines: y = -k * x + k*k
    std::vector<i64> prefix = {0, 5, 18, 40, 70, 110};
    for (int k = 1; k < (int)prefix.size(); ++k) {
        i64 m = -k;
        i64 b = prefix[k] + k * k;
        cht.add_line(m, b);
    }
    for (int x = 1; x <= 5; ++x) {
        std::cout << "Query x=" << x << ": " << cht.query(x) << "\\n";
    }
    return 0;
}


