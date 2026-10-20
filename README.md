# Group 2 - Energy-Aware Task Scheduling in Edge Computing (TV4: Duong)

## Cau truc repo

```
Group2_EdgeDP/
+-- src/
|   +-- baseline_dp.cpp       # DP(i,j) co ban O(N^2 M)
|   +-- dnc_dp.cpp            # Divide-and-Conquer DP, O(M N log N)
|   +-- cht.cpp               # Convex Hull Trick (deque)
|   +-- cht_li_chao.cpp       # Li Chao segment tree
|   +-- knuth.cpp             # Knuth optimization
+-- tests/
|   +-- test_baseline.cpp     # 1000 brute vs baseline (Catch2)
|   +-- test_dnc.cpp          # baseline vs D&C (Catch2)
|   +-- test_knuth.cpp        # Knuth smoke test
|   +-- CMakeLists.txt
+-- scripts/
|   +-- brute_force.py        # reference cho N<=12
|   +-- baseline_dp.py        # baseline O(N^2 M)
|   +-- prototype.py          # sinh test ngau nhien
|   +-- heuristics.py         # RR / MEF / EDF / GES
|   +-- energy_compare.py     # xuat results/energy.csv
+-- results/                  # CSV so sanh nang luong / latency
+-- docs/
|   +-- DP_OPTIMIZATION_NOTES.md
|   +-- COVERAGE_REPORT.md
+-- .github/workflows/ci.yml  # GitHub Actions (task 6.06)
+-- README.md
```

## Tai lap nhanh

```bash
# C/C++ test
cmake -B build && cmake --build build && ctest --test-dir build -V

# Pipeline Python
python scripts/energy_compare.py
# -> results/energy.csv
```

## Lich trinh TV4 (theo bang ke hoach 7 tuan)

| ID | Task | Bắt đầu | Hạn chót | Trạng thái |
|----|------|---------|---------|-----------|
| 1.07 | Ôn kỹ thuật tối ưu DP + bài mẫu | 06/10 | 11/10 | OK |
| 2.04 | Brute force N≤12 + prototype Python | 12/10 | 16/10 | OK |
| 2.05 | Unit test đợt 1 (Catch2, 1000 test) | 15/10 | 18/10 | OK |
| 3.05 | Pseudo-code D&C & CHT | 21/10 | 24/10 | OK |
| 4.01 | Cài D&C Optimization | 26/10 | 30/10 | OK |
| 5.04 | So sánh năng lượng với 4 heuristic | 04/11 | 08/11 | OK |
| 6.06 | Thiết lập CI GitHub Actions | 09/11 | 13/11 | OK |
| 7.02 | Hoàn thiện unit test biên + coverage | 16/11 | 19/11 | OK |
