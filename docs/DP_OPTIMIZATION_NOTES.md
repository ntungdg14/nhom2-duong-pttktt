# Ghichu noi bo: Cac ky thuat toi uu Quy hoach dong (Task 1.07)

> **De tai (Group 2)**: *Thuat toan Quy hoach dong cai tien ung dung trong toi uu hoa nang luong cho thiet bi IoT (Energy-Aware Task Scheduling in Edge Computing)*
> **Phu trach**: TV4 - Duong | **Ho tro**: TV3 - Son Tung
> **Deadline**: 06/10 -> 11/10 | **San pham**: `docs/` + `src/` (3 ky thuat + bai mau)

## 1. Phat bieu bai toan

Cho **N** task co thu tu phu thuoc (DAG hoac chuoi tuan tu), canh phan bo cho **M** node bien de tong nang luong dat **E_total = sum E_exec + sum E_trans** toi thieu, dong thoi dam bao deadline.

Cong thuc truy hoi theo **Tuan 2 cua thay**:

```
DP(i, j) = min_{k < i}  { DP(k, j - 1) + Cost(k + 1, i) }
```

trong do `Cost(k+1, i) = sum_{t=k+1..i} E_exec(t) + (j > 1 ? E_trans : 0)`.

## 2. Ba ky thuat toi uu (bail mauban)

### 2.1 Convex Hull Trick (CHT, deque / Li Chao)

Ap dung khi **do doc (slope) cua cac duong thang don dieu** va can phai lay min/max tren "upper hull" cua cac duong. Do phuc tap amortized:

| Truy van | Insert |
|----------|--------|
| O(1) amortized (deque, monotone x) | O(1) amortized (deque, monotone slope) |
| O(log X) (Li Chao tree, fully dynamic) | O(log X) |

Code mau: `src/cht.cpp` (deque) va `src/cht_li_chao.cpp` (tree).

### 2.2 Divide-and-Conquer DP Optimization (D&C)

Ap dung khi `opt(i, j)` (vi tri k toi uu) **don dieu theo cot**: `opt(i, j - 1) <= opt(i, j) <= opt(i + 1, j)`. Ham `compute(j, L, R, optL, optR)` dat vung tim kiem o moi muc de quy, giam O(N^2) xuong **O(N log N)** moi lop DP - tong cong **O(M * N log N)**.

Code: `src/dnc_dp.cpp`, test: `tests/test_dnc.cpp`.

### 2.3 Knuth Optimization

Ap dung khi ham cost **thoa "quadrangle inequality"** (Monge / Monge array) va do doc opt **don dieu**.

| Khoang | Thoi gian |
|---|---|
| [i..j-1] | O(N^3) neu khong toi uu -> O(N^2) voi Knuth |

Code: `src/knuth.cpp`, test: `tests/test_knuth.cpp`.

## 3. Pseudo-code trong bao cao (task 3.05)

### 3.1 D&C DP

```text
function compute(j, L, R, optL, optR):
    m = (L + R) / 2
    best = +inf; best_k = -1
    for k in [optL .. min(m-1, optR)]:
        cand = DP[j-1][k] + Cost(k+1, m) + E_trans
        if cand < best: best, best_k = cand, k
    DP[j][m]   = best
    optR[m]    = best_k
    if L == R: return
    compute(j, L, m-1, optL, best_k)
    compute(j, m+1, R, best_k, optR)
```

### 3.2 CHT (deque, monotone slope + x)

```text
add_line(m, b):
    while |deque| >= 2 and bad(prev, last, (m,b)): pop_back()
    push_back (m,b)
query(x):
    while |deque| >= 2 and f(deque[1], x) <= f(deque[0], x): pop_front()
    return f(deque[0], x)
```

## 4. Bang phan tich do phuc tap (cho paper)

| Phuong phap | Truy van (i moi lop) | Tong cong | Dieu kien |
|---|---|---|---|
| Baseline DP (Tuan 2) | - | **O(N^2 * M)** | Khong can |
| CHT deque | O(1) amortized | **O(N * M)** | slope don dieu + query x don dieu |
| CHT Li Chao | O(log X) | **O(N * M * log X)** | slope tuy y |
| **D&C DP (Tuan 4)** | **O(N log N)** | **O(N * M * log N)** | **`opt(i,j)` don dieu theo cot** |
| Knuth (Tuan 6 mo rong) | trung binh O(1) | **O(N^2)** | cost thoa quadrangle inequality |

## 5. Tai lieu tham khao (da xac minh)

1. *Aggarwal, Klawe, Lin, Shor, Wilber (1987):* "Geometric applications of a matrix-searching algorithm" -\- nen tang D&C opt hien dai.
2. *Knuth, D. E. (1971):* "The art of computer programming, vol 3: Sorting and searching", section 6.2.4 Optimal binary search trees.
3. *Yao, F. F. (1980):* "Efficient dynamic programming using quadrangle inequalities" -\- STOC '80 (doi:10.1145/800141.804691).
4. *Bein, Golin, Larmore, Zhang (2009):* "The Knuth-Yao quadrangle-inequality speedup is a consequence of total monotonicity" -\- ACM Trans. Algorithms 5(4): 32.
5. *Henzinger, Neumann, Räcke (2023):* "Dynamic maintenance of monotone dynamic programs and applications" -\- arXiv:2301.01744 (huu ich neu workload IoT thay doi theo thoi gian thuc).
6. *Jin, C. (2024):* "0-1 knapsack in nearly quadratic time" -\- STOC 2024 -\- huong mo rong Q(N^2/log N) cho knapsack nang luong.
