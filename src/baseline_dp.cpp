// baseline_dp.cpp
// "Lap cong thuc truy hoi DP va cai dat giai thuat Quy hoach dong co ban voi do phuc tap O(N^2 * M)"
// DP(i, j) = min_{k < i} { DP(k, j - 1) + Cost(k + 1, i) }
// Cost(k + 1, i) = E_exec(k+1..i) + (j > 1 ? E_trans : 0)

#include <vector>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <iomanip>

using i64 = long long;
const i64 INF64 = (i64)4e18;

class BaselineDP {
public:
    // Prefix of execution energy
    std::vector<i64> prefix;
    // Energy per transmission (between consecutive groups)
    int64_t E_trans;
    // DP[j][i]  (j in [1..M], i in [1..N])
    std::vector<std::vector<i64>> dp;
    // opt[j][i] for reconstruction
    std::vector<std::vector<int>> optk;

    BaselineDP(const std::vector<int64_t>& exec, int M, int64_t E_trans_)
        : E_trans(E_trans_), M(M) {
        int N = (int)exec.size();
        prefix.assign(N + 1, 0);
        for (int i = 1; i <= N; ++i) prefix[i] = prefix[i-1] + exec[i-1];

        dp.assign(M + 1, std::vector<i64>(N + 1, INF64));
        optk.assign(M + 1, std::vector<int>(N + 1, -1));

        // Base: 1 node, cost of executing all = prefix[N] (no transmission when M=1)
        for (int i = 1; i <= N; ++i) {
            dp[1][i] = prefix[i];
            optk[1][i] = 0;
        }

        for (int j = 2; j <= M; ++j) {
            for (int i = j; i <= N; ++i) {
                i64 best = INF64;
                int bk = -1;
                for (int k = j - 1; k <= i - 1; ++k) {
                    // task group [k+1..i] runs on node j
                    i64 cand = dp[j-1][k] + (prefix[i] - prefix[k]) + E_trans;
                    if (cand < best) {
                        best = cand;
                        bk = k;
                    }
                }
                dp[j][i] = best;
                optk[j][i] = bk;
            }
        }
    }

    i64 total_energy() const {
        return dp[M][prefix.size() - 1];
    }

    void reconstruct(std::vector<std::pair<int,int>>& groups) const {
        int i = (int)prefix.size() - 1;
        for (int j = M; j >= 1; --j) {
            int k = optk[j][i];
            int prev_k = (k == 0 ? 0 : k);
            groups.emplace_back(prev_k + 1, i);
            i = prev_k;
        }
        std::reverse(groups.begin(), groups.end());
    }
};

// Driver: read from stdin
// Input:
//   N M E_trans
//   e_1 e_2 ... e_N
int main() {
    int N, M;
    int64_t E_trans;
    if (!(std::cin >> N >> M >> E_trans)) {
        std::cerr << "Usage: N M E_trans\\n e_1..e_N\\n";
        return 1;
    }
    std::vector<int64_t> exec(N);
    for (int i = 0; i < N; ++i) std::cin >> exec[i];

    BaselineDP solver(exec, M, E_trans);
    std::cout << "Total E = " << solver.total_energy() << "\\n";

    std::vector<std::pair<int,int>> groups;
    solver.reconstruct(groups);
    std::cout << "Group assignment (tasks -> node index):\\n";
    for (size_t j = 0; j < groups.size(); ++j) {
        std::cout << "  Node " << (j+1) << ": tasks [" << groups[j].first
                  << ", " << groups[j].second << "]\\n";
    }
    return 0;
}
