// dnc_dp.cpp
// Divide-and-Conquer DP Optimization (D&C opt) for the same recurrence.
// Roadmap alignment (Tuan 4 in teacher's plan):
//   "Cai dat ky thuat cai tien vao giai thuat... giam thooi gian tinh cho moi
//    buoc chuyen trang thai xuong O(1) hoac O(log N)"
//
// Requires: opt(i,j) <= opt(i, j+1) for each fixed i  (monotone by column)
// Reduces the j-th DP layer from O(j^2) to O(j log j).  Total complexity:
//   O(M * N * log N).
//
// Verification: Aggarwal-Klawe-Lin-Shor-Wilber (1987) "Geometric applications
// of a matrix-searching algorithm" - the SMAWK-based monotone matrix min
// algorithm; also surveyed in CP-Algos / KACTL "Divide and Conquer DP".

#include <vector>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <iostream>

using i64 = long long;
const i64 INF64 = (i64)4e18;

class DNCDPSolver {
public:
    int N, M;
    int64_t E_trans;
    std::vector<i64> prefix;
    std::vector<std::vector<i64>> dp;
    std::vector<std::vector<int>> optk;

    DNCDPSolver(const std::vector<int64_t>& exec, int M_, int64_t E_trans_)
        : N((int)exec.size()), M(M_), E_trans(E_trans_) {
        prefix.assign(N + 1, 0);
        for (int i = 1; i <= N; ++i) prefix[i] = prefix[i-1] + exec[i-1];

        dp.assign(M + 1, std::vector<i64>(N + 1, INF64));
        optk.assign(M + 1, std::vector<int>(N + 1, -1));

        for (int i = 1; i <= N; ++i) {
            dp[1][i] = prefix[i];
            optk[1][i] = 0;
        }
        // dp[1][i] is already initialized above. Do not recompute layer 1.
        for (int j = 2; j <= M; ++j) compute_layer(j);
    }

    // Reuses the standard D&C recurrence:
    //   Let opt(j, i) = argmin_k { dp[j-1][k] + Cost(k+1, i) + E_trans }
    //   Monotonicity step bounds the search window by recursing on [L..M].
    void compute(int j, int L, int R, int optL, int optR) {
        int m = (L + R) >> 1;
        i64 best = INF64;
        int bk = -1;
        int k_end = std::min(m - 1, optR);
        for (int k = optL; k <= k_end; ++k) {
            if (dp[j-1][k] == INF64) continue;
            i64 cand = dp[j-1][k] + (prefix[m] - prefix[k]) + E_trans;
            if (cand < best) {
                best = cand;
                bk = k;
            }
        }
        dp[j][m] = best;
        optk[j][m] = bk;
        if (L == R) return;
        if (L <= m - 1) compute(j, L, m - 1, optL, bk);
        if (m + 1 <= R) compute(j, m + 1, R, bk, optR);
    }

    void compute_layer(int j) {
        // Layer 1 is already filled; run algorithm for j-th layer over i=j..N
        // The opt lower bound is optL = j - 1 (you need at least j-1 tasks per group).
        // The recurse window at outer level is [j, N], with global opt range [j-1, N-1].
        // Following the canonical D&C opt code (see CP-Algos):
        compute(j, j, N, j - 1, N - 1);
    }

    i64 total_energy() const { return dp[M][N]; }

    void reconstruct(std::vector<std::pair<int,int>>& groups) const {
        int i = N;
        for (int j = M; j >= 1; --j) {
            int k = optk[j][i];
            int pk = (k == 0 ? 0 : k);
            groups.emplace_back(pk + 1, i);
            i = pk;
        }
        std::reverse(groups.begin(), groups.end());
    }
};

int main() {
    int N, M;
    int64_t E_trans;
    if (!(std::cin >> N >> M >> E_trans)) {
        std::cerr << "Usage: N M E_trans\n e_1..e_N\n";
        return 1;
    }
    if (N <= 0 || M <= 0 || M > N) {
        std::cerr << "Require 1 <= M <= N.\n";
        return 1;
    }
    std::vector<int64_t> exec(N);
    for (int i = 0; i < N; ++i) std::cin >> exec[i];

    DNCDPSolver solver(exec, M, E_trans);
    std::cout << "D&C total E = " << solver.total_energy() << "\n";

    std::vector<std::pair<int,int>> groups;
    solver.reconstruct(groups);
    for (size_t j = 0; j < groups.size(); ++j) {
        std::cout << "Node " << (j+1) << ": [" << groups[j].first
                  << ", " << groups[j].second << "]\n";
    }
    return 0;
}
