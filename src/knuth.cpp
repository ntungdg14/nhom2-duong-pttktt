// knuth.cpp
// Knuth optimization: O(N^2) optimal binary search tree-like recurrences.
// It applies when the cost function satisfies the quadrangle inequality:
//   C[a][c] + C[b][d] <= C[a][d] + C[b][c]   (abrow <= acdr)
// and the resulting opt[i][j] is monotone:
//   opt[i][j-1] <= opt[i][j] <= opt[i+1][j]
// This yields O(N^2) total time for DP[i][j] = min over k of { DP[i][k-1] +
// DP[k+1][j] + C[i][j] }.
//
// Reference: Knuth 1971 "Optimal binary search trees"; Yao 1980 STOC.

#include <vector>
#include <algorithm>
#include <cstdint>
#include <iostream>

using i64 = long long;
const i64 INF64 = (i64)4e18;

class Knuth {
public:
    int N;
    std::vector<std::vector<i64>> C;       // quadrangle-inequality holding
    std::vector<std::vector<i64>> dp;
    std::vector<std::vector<int>>  opt;

    Knuth(const std::vector<std::vector<i64>>& C_) {
        N = (int)C_.size() - 1;            // C is 1-indexed, size N+1
        C = C_;
        dp.assign(N + 2, std::vector<i64>(N + 2, INF64));
        opt.assign(N + 2, std::vector<int>(N + 2, 0));
        for (int i = 1; i <= N; ++i) { dp[i][i] = C[i][i]; opt[i][i] = i; }
        build();
    }

    void build() {
        for (int len = 2; len <= N; ++len) {
            for (int i = 1; i + len - 1 <= N; ++i) {
                int j = i + len - 1;
                i64 best = INF64;
                int bk = -1;
                for (int k = opt[i][j-1]; k <= opt[i+1][j]; ++k) {
                    i64 cand = dp[i][k-1] + dp[k+1][j] + C[i][j];
                    if (cand < best) { best = cand; bk = k; }
                }
                dp[i][j] = best;
                opt[i][j] = bk;
            }
        }
    }
};

// Toy driver: build a convex quadrangle-inequality-holding cost C.
int main() {
    // Construct C[i][j] = sum_{t=i..j} freq[t]
    std::vector<i64> freq = {0, 5, 2, 7, 3, 9};
    int N = 5;
    std::vector<std::vector<i64>> C(N + 2, std::vector<i64>(N + 2, 0));
    for (int i = 1; i <= N; ++i) {
        i64 s = 0;
        for (int j = i; j <= N; ++j) { s += freq[j]; C[i][j] = s; }
    }
    Knuth kn(C);
    for (int i = 1; i <= N; ++i) {
        for (int j = i; j <= N; ++j)
            std::cout << "dp[" << i << "][" << j << "]=" << kn.dp[i][j] << " ";
        std::cout << "\\n";
    }
    return 0;
}
