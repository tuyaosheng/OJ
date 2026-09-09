```cpp
int maxSize = 1;  // 至少每台设备自己算一个大小为 1 的分组

void merge(int x, int y) {
    x = find(x); y = find(y);
    if (x == y) return;
    if (sz[x] > sz[y]) swap(x, y);
    fa[x] = y;
    sz[y] += sz[x];
    maxSize = max(maxSize, sz[y]);
}
```

`maxSize` 只需要在**每次真正发生合并**之后更新一次（拿合并后新根的 `sz[y]` 去比较），不需要在最后再扫一遍全部 $n$ 个点找最大值——这和例2维护"集合总数"是同一个思路：能在合并的瞬间顺手更新的全局信息，就不要留到最后再重新计算一遍。
