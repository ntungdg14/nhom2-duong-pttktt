// test_dnc.cpp  - D&C DP equality with baseline
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_test_macros.hpp>
#include <random>
#include "../src/baseline_dp.cpp"
#include "../src/dnc_dp.cpp"

TEST_CASE("D&C DP agrees with baseline on random instances", "[dnc]") {
    std::mt19937 rng(7);
    std::uniform_int_distribution<int> N_dist(1, 200);
    std::uniform_int_distribution<int> M_dist(1, 10);
    std::uniform_int_distribution<long long> E_trans_dist(0, 100);
    std::uniform_int_distribution<long long> e_dist(1, 1000);
    for (int t = 0; t < 200; ++t) {
        int N = N_dist(rng);
        int M = std::min(M_dist(rng), N);
        long long E_trans = E_trans_dist(rng);
        std::vector<long long> e(N);
        for (auto& v : e) v = e_dist(rng);
        BaselineDP b(e, M, E_trans);
        DNCDPSolver d(e, M, E_trans);
        REQUIRE(b.total_energy() == d.total_energy());
    }
}

int main(int argc, char** argv) { return Catch::Session().run(argc, argv); }
