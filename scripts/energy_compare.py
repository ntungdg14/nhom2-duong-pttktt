"""energy_compare.py - So sanh nang luong giua DP toi uu va 4 heuristic.

Sinh workload IoT (N mac dinh 10^3, 10^4) -> chay DP + 4 heuristic ->
xuat results/energy.csv theo yeu cau task 5.04 (Energy Saved %, vi pham deadline,
Speedup ratio so voi baseline DP).
"""
from __future__ import annotations
import csv
import os
import random
import time
import heuristics as H
import baseline_dp as BD

OUT_CSV = os.path.join(os.path.dirname(__file__), "..", "results", "energy.csv")

def workload_iot(N, seed):
    rnd = random.Random(seed)
    e = [rnd.randint(1, 20) for _ in range(N)]   # E_exec / task
    deadlines = [int(N / max(1, M) * 1.2) for M in range(1, N + 1)]  # placeholder
    return e, deadlines


def run_one(N, M, seed):
    e, _ = workload_iot(N, seed)
    eT = 7
    t0 = time.perf_counter(); dp_E, dp_groups = BD.baseline_dp(e, M, eT)
    t_dp = time.perf_counter() - t0

    t0 = time.perf_counter(); rr_E, _ = H.round_robin(e, M, eT); t_h = time.perf_counter() - t0

    t0 = time.perf_counter(); mef_E, _, mef_viol = H.min_energy_first(e, M, eT); t_h += time.perf_counter() - t0

    t0 = time.perf_counter(); edf_E, _ = H.earliest_deadline_first(e, M, eT, None); t_h += time.perf_counter() - t0

    t0 = time.perf_counter(); ges_E, _ = H.greedy_energy_saved(e, M, eT); t_h += time.perf_counter() - t0
    t_h /= 4

    saved_rr = 100 * (rr_E - dp_E) / max(rr_E, 1)
    saved_mef = 100 * (mef_E - dp_E) / max(mef_E, 1)
    saved_edf = 100 * (edf_E - dp_E) / max(edf_E, 1)
    saved_ges = 100 * (ges_E - dp_E) / max(ges_E, 1)
    return {
        "N": N, "M": M,
        "E_DP": dp_E,
        "E_RR": rr_E, "E_MEF": mef_E, "E_EDF": edf_E, "E_GES": ges_E,
        "Saved_RR_pct": round(saved_rr, 2),
        "Saved_MEF_pct": round(saved_mef, 2),
        "Saved_EDF_pct": round(saved_edf, 2),
        "Saved_GES_pct": round(saved_ges, 2),
        "Violations_MEF_pct": round(100 * mef_viol / max(M, 1), 2),
        "Time_DP_s": round(t_dp, 5),
        "Time_Heuristic_avg_s": round(t_h, 5),
        "Speedup_vs_BruteForce": round(t_dp / max(t_h, 1e-6), 3),
    }

def main():
    os.makedirs(os.path.dirname(OUT_CSV), exist_ok=True)
    rows = []
    for N in (100, 1000, 5000):
        for M in (2, 5, max(2, N // 20)):
            for seed in (1, 2, 3):
                if M > N: continue
                rows.append(run_one(N, M, seed))

    fields = list(rows[0].keys())
    with open(OUT_CSV, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader(); w.writerows(rows)

    print(f"Wrote {len(rows)} rows -> {OUT_CSV}")
    for r in rows[:5]:
        print(r)


if __name__ == "__main__":
    main()
