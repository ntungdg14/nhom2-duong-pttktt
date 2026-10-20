"""prototype.py - San sinh test ngau nhien va so sanh brute vs baseline_dp.

Dung de kiem tra nhanh cong thuc truoc khi merge baseline_dp.cpp (task 2.03).
"""
from __future__ import annotations
import argparse
import random
import time
import brute_force as bf
import baseline_dp as bd

def main():
    p = argparse.ArgumentParser()
    p.add_argument("--N", type=int, default=10)
    p.add_argument("--M", type=int, default=4)
    p.add_argument("--seed", type=int, default=42)
    p.add_argument("--trials", type=int, default=50)
    args = p.parse_args()

    rnd = random.Random(args.seed)
    print(f"Random workload: N={args.N}, M={args.M}, trials={args.trials}")
    fails = 0
    for t in range(args.trials):
        e = [rnd.randint(1, 100) for _ in range(args.N)]
        et = rnd.randint(0, 30)
        t0 = time.perf_counter()
        b_exp = bf.brute(e, et)
        t_brute = time.perf_counter() - t0
        t0 = time.perf_counter()
        b_got, _ = bd.baseline_dp(e, args.M, et)
        t_dp = time.perf_counter() - t0
        ok = (b_exp == b_got) if args.M <= args.N else True  # neu M<=N
        if not ok: fails += 1
        print(f"[{t:02d}] exp={b_exp:6} got={b_got:6} Etrans={et:3} "
              f"brute {t_brute:.3f}s dp {t_dp:.3f}s -> {'PASS' if ok else 'FAIL'}")
    print(f"{'ALL PASS' if fails == 0 else f'{fails} FAILED'}")


if __name__ == "__main__":
    main()
