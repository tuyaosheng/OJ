```cpp
int fa[100005], sz[100005];
int count_;  // 当前集合总数

void init(int n) {
    for (int i = 1; i <= n; i++) { fa[i] = i; sz[i] = 1; }
    count_ = n;
}

int find(int x) {
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}

void merge(int x, int y) {
    x = find(x); y = find(y);
    if (x == y) return;          // 已经是同一个集合，合并不产生新效果
    if (sz[x] > sz[y]) swap(x, y);
    fa[x] = y;
    sz[y] += sz[x];
    count_--;                    // 两个集合变成一个，总数减一
}
```

维护一个全局计数器 `count_`，初始等于 $n$（每人一个圈子），每次**真正发生合并**（两个根不同）就减一，比最后再扫一遍数组统计"有多少个不同的根"更直接，而且是 $O(1)$ 增量维护，不需要额外的 $O(n)$ 扫描。

一定要先判断 `x == y`（两点已经在同一个集合）的情况，如果不判断直接减一，"合并同一个圈子里的两个人"会被误算成产生了新的合并，导致计数错误。
