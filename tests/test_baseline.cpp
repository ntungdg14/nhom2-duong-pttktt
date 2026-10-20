// test_baseline.cpp
// Catch2 v3 (single-header) - baseline DP vs brute force on 1000 random
// small instances.  Aligns with plan task 2.05.

#define CATCH_CONFIG_RUNNER
#include <catch2/catch_test_macros.hpp>
#include <random>
#include <vector>
#include <numeric>
#include <functional>

#include "../src/baseline_dp.cpp"

static long long brute(const std::vector<long long>& e, long long E_trans, int M) {
    int N = (int)e.size();
    if (M == 1) {
        long long s = 0; for (auto v : e) s += v; return s;
    }
    long long best = (long long)4e18;
    std::function<void(int,int)> dfs = [&](int start, int left_groups) {
        if (left_groups == 1) {
            long long s = std::accumulate(e.begin()+start, e.end(), 0LL);
            s += (M - 1) * E_trans;
            best = std::min(best, s);
            return;
        }
        for (int c = start; c <= N - left_groups; ++c) {
            dfs(c + 1, left_groups - 1);
        }
    };
    dfs(0, M);
    return best;
}

TEST_CASE("baseline agrees with brute on 1000 random instances", "[baseline]") {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> N_dist(1, 12);
    std::uniform_int_distribution<int> M_dist(1, 5);
    std::uniform_int_distribution<long long> E_trans_dist(0, 50);
    std::uniform_int_distribution<long long> e_dist(1, 100);

    for (int t = 0; t < 1000; ++t) {
        int N = N_dist(rng);
        int M = std::min(M_dist(rng), N);
        long long E_trans = E_trans_dist(rng);
        std::vector<long long> e(N);
        for (auto& v : e) v = e_dist(rng);
        BaselineDP solver(e, M, E_trans);
        REQUIRE(solver.total_energy() == brute(e, E_trans, M));
    }
}

TEST_CASE("edge N=1", "[edge]") {
    std::vector<long long> e = {7};
    BaselineDP s(e, 1, 10);
    REQUIRE(s.total_energy() == 7);
}
TEST_CASE("edge M=1 ignores transmission", "[edge]") {
    std::vector<long long> e = {1, 2, 3, 4};
    BaselineDP s(e, 1, 1000);
    REQUIRE(s.total_energy() == 10);
}
TEST_CASE("edge M=N one task per node", "[edge]") {
    std::vector<long long> e = {3, 1, 4, 2};
    BaselineDP s(e, 4, 5);
    REQUIRE(s.total_energy() == 10 + 3 * 5);
}

int main(int argc, char** argv) { return Catch::Session().run(argc, argv); }
