"""brute_force.py - Reference solver for N<=12 only.

Phat bieu: DP(i,j) = min_{k<i} { DP(k, j-1) + Cost(k+1, i) + (j>1 ? E_trans:0) }

Tham so: nhom 2, task 2.04.
"""
from __future__ import annotations
import argparse
from itertools import combinations

def brute(exec_e, e_trans):
    n = len(exec_e)
    m_max = n
    best = float('inf')
    # Casses a brute-force chia day task thanh cac nhom (chi can M bat ky, tuy nhien
    # bai toan goc cho phep chon M <= N; chung ta quet tat ca M).
    for m in range(1, n + 1):
        for cuts in combinations(range(1, n), m - 1):
            cuts = (0,) + cuts + (n,)
            cost = 0
            for k in range(m):
                seg = sum(exec_e[cuts[k]:cuts[k+1]])
                cost += seg + (e_trans if k > 0 else 0)
            if cost < best:
                best = cost
    return best


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--seed", type=int, default=1)
    p.add_argument("--N", type=int, default=8)
    p.add_argument("--Etrans", type=int, default=5)
    args = p.parse_args()
    import random
    rnd = random.Random(args.seed)
    e = [rnd.randint(1, 50) for _ in range(args.N)]
    print("Tasks:", e, "Etrans:", args.Etrans)
    print("Brute force min E =", brute(e, args.Etrans))


if __name__ == "__main__":
    main()
