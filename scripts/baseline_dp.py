"""baseline_dp.py - O(N^2 * M) baseline Dynamic Programming.

Cong thuc truy hoi theo huong dan cua thay:
    DP(i, j) = min_{k < i} { DP(k, j - 1) + Cost(k + 1, i) }
trong do Cost(k+1, i) = tong E_exec(k+1..i) + (j>1 ? E_trans : 0).
"""
from __future__ import annotations
import sys

INF = 10 ** 18

def baseline_dp(exec_e, M, e_trans):
    n = len(exec_e)
    pref = [0] * (n + 1)
    for i in range(1, n + 1):
        pref[i] = pref[i-1] + exec_e[i-1]

    dp = [[INF] * (n + 1) for _ in range(M + 1)]
    opt = [[0] * (n + 1) for _ in range(M + 1)]

    for i in range(1, n + 1):
        dp[1][i] = pref[i]
        opt[1][i] = 0
    for j in range(2, M + 1):
        for i in range(j, n + 1):
            best, bk = INF, -1
            for k in range(j - 1, i):
                cand = dp[j-1][k] + (pref[i] - pref[k]) + e_trans
                if cand < best:
                    best, bk = cand, k
            dp[j][i] = best
            opt[j][i] = bk

    if dp[M][n] >= INF:
        raise ValueError("Feasible solution not found (need M <= N).")

    # Reconstruct grouping
    groups = []
    i = n
    for j in range(M, 0, -1):
        k = opt[j][i]
        prev = 0 if k == 0 else k
        groups.append((prev + 1, i))
        i = prev
    groups.reverse()
    return dp[M][n], groups


def main():
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 10
    M = int(sys.argv[2]) if len(sys.argv) > 2 else 3
    et = int(sys.argv[3]) if len(sys.argv) > 3 else 7
    e = list(map(int, sys.argv[4:4 + n])) if len(sys.argv) >= 4 + n else [i + 1 for i in range(n)]
    E, groups = baseline_dp(e, M, et)
    print(f"Total E = {E}")
    for idx, (a, b) in enumerate(groups, 1):
        print(f"  Node {idx}: tasks [{a}, {b}] = {sum(e[a-1:b])}")


if __name__ == "__main__":
    main()
