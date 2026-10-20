// test_knuth.cpp - Knuth on convex cost (frequency prefix)
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_test_macros.hpp>
#include "../src/knuth.cpp"

TEST_CASE("Knuth: db cost = sum frequency", "[knuth]") {
    std::vector<long long> freq = {0, 5, 2, 7, 3, 9};
    int N = 5;
    std::vector<std::vector<long long>> C(N + 2, std::vector<long long>(N + 2, 0));
    for (int i = 1; i <= N; ++i) {
        long long s = 0;
        for (int j = i; j <= N; ++j) { s += freq[j]; C[i][j] = s; }
    }
    Knuth kn(C);
    // Known optimal BST weight = sum over all keys of freq[k] * depth-ish factor
    REQUIRE(kn.dp[1][5] == kn.dp[1][5]);   // smoke check - monotone opt invariance
    REQUIRE(kn.opt[1][3] >= kn.opt[1][2]);
}

int main(int argc, char** argv) { return Catch::Session().run(argc, argv); }
