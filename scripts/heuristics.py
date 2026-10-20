"""heuristics.py - 4 heuristic doi chieu theo task 2.08.

DTV4 dung 4 thuat toan nay trong task 5.04 (Energy compare).
"""
from __future__ import annotations

def round_robin(exec_e, M, e_trans):
    """Chia task theo thu tu: nhom 1..M vong tron.

    Transmission giua cac node lien tiep, deadline violation mac dinh 0.
    """
    n = len(exec_e)
    groups = [[] for _ in range(M)]
    for i, x in enumerate(exec_e):
        groups[i % M].append(x)
    cost = sum(sum(g) for g in groups) + (M - 1) * e_trans
    return cost, groups


def min_energy_first(exec_e, M, e_trans):
    """Lay task co E_exec nho nhat truoc, gom vao node hien tai den khi tran deadline.

    Dung deadline gia dinh = sum(e) / M * 1.5 (cau hinh don gian).
    """
    sorted_e = sorted(exec_e)
    deadline = sum(sorted_e) / max(M, 1) * 1.5
    groups, cur, cur_sum = [], [], 0
    for x in sorted_e:
        if cur and cur_sum + x > deadline:
            groups.append(cur); cur, cur_sum = [], 0
        cur.append(x); cur_sum += x
    if cur: groups.append(cur)
    # Phan bo vao M node, neu thua thi cho vao node cuoi
    while len(groups) < M: groups.append([])
    cost = sum(sum(g) + (e_trans if i > 0 else 0) for i, g in enumerate(groups))
    violation = sum(1 for g in groups if sum(g) > deadline)
    return cost, groups, violation


def earliest_deadline_first(exec_e, M, e_trans, deadlines):
    """EDF: sap xep theo deadline tang dan truoc khi chia round-robin.

    deadline mac dinh neu khong du cung cap = index * 2.
    """
    if deadlines is None: deadlines = [(i + 1) * 2 for i in range(len(exec_e))]
    pairs = sorted(zip(exec_e, deadlines), key=lambda t: t[1])
    sorted_e = [p[0] for p in pairs]
    return round_robin(sorted_e, M, e_trans)


def greedy_energy_saved(exec_e, M, e_trans):
    """Lay lon nhat truoc, chen vao nhom co tong hien tai nho nhat."""
    n = len(exec_e)
    sorted_e = sorted(exec_e, reverse=True)
    groups = [[] for _ in range(M)]
    for x in sorted_e:
        idx = min(range(M), key=lambda i: sum(groups[i]))
        groups[idx].append(x)
    cost = sum(sum(g) + (e_trans if i > 0 else 0) for i, g in enumerate(groups))
    return cost, groups
