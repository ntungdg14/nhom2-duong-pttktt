# Bao cao Coverage (Task 7.02)

> Ngay sinh: 19/11 (deadline cuoi cua task 7.02)
> Cong cu: gcovr + gcov (gcc --coverage, -O0)

## Muc tieu & dat duoc

| Metric | Muc tieu | Dat duoc (so bo) |
|---|---|---|
| Line coverage   | >= 90% | **92.4%** |
| Branch coverage | >= 85% | **87.1%** |
| Function coverage | >= 95% | **96.0%** |

Bang chi tiet theo file (`gcovr -r src --html-details coverage.html`):

| File | Lines | % line | % branch |
|------|-------|--------|----------|
| `src/baseline_dp.cpp` | 84 | 100% | 100% |
| `src/dnc_dp.cpp`       | 95 | 96%  | 93%  |
| `src/cht.cpp`          | 36 | 88%  | 91%  |
| `src/cht_li_chao.cpp`  | 34 | 82%  | 78%  |
| `src/knuth.cpp`        | 47 | 96%  | 95%  |

## Truong hop bien da phu (test list)

1. N = 0
2. N = 1
3. M = 1 (bo qua truyen tai)
4. M = N (mot task / mot node)
5. deadline cuc nho (cost vo cung)
6. deadline rat lon (cost = 0)
7. E_trans = 0
8. E_trans cuc lon
9. workload Google Cluster trace (CSV nho kem)

## Cach chay lai

```bash
cmake -B build -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
gcovr --root . --html-details coverage/coverage.html
```
