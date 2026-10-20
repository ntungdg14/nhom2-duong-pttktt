Cau truc repo
nhom2-duong-pttktt/
src/
  baseline_dp.cpp       # DP(i,j) co ban O(N^2 M)
  dnc_dp.cpp            # Divide-and-Conquer DP, O(M N log N)
  cht.cpp               # Convex Hull Trick (deque)
  cht_li_chao.cpp       # Li Chao segment tree
  knuth.cpp             # Knuth optimization
  
tests/
  test_baseline.cpp     # 1000 brute vs baseline (Catch2)
  test_dnc.cpp          # baseline vs D&C (Catch2)
  test_knuth.cpp        # Knuth smoke test
  CMakeLists.txt
  
scripts/
  brute_force.py        # reference cho N<=12
  baseline_dp.py        # baseline O(N^2 M)
  prototype.py          # sinh test ngau nhien
  heuristics.py         # RR / MEF / EDF / GES
  energy_compare.py     # xuat results/energy.csv

docs/
  DP_OPTIMIZATION_NOTES.md
  COVERAGE_REPORT.md
  .github/workflows/ci.yml  # GitHub Actions
README.md

Tai lap nhanh

```bash
# C/C++ test
cmake -B build && cmake --build build && ctest --test-dir build -V

# Pipeline Python
python scripts/energy_compare.py
# -> results/energy.csv
```
