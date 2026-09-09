最基础也最常用的写法——**路径压缩**版本的 `find`：查找根节点的同时，把沿途经过的每个点都**直接**挂到根上，下次再查这些点就是 $O(1)$：

```cpp
int fa[100005];

void init(int n) {
    for (int i = 1; i <= n; i++) fa[i] = i;  // 一开始每个人的"老大"就是自己
}

int find(int x) {
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);  // 路径压缩：顺手把 x 直接接到根上
}

void merge(int x, int y) {
    fa[find(x)] = find(y);
}
```

如果不加路径压缩，最坏情况下（比如一直把新点往同一条链上接）`find` 会退化成 $O(n)$，$m$ 次操作总共 $O(nm)$，$n, m$ 达到 $10^5 \sim 2\times10^5$ 时会超时；加了路径压缩之后，树会不断被"拍扁"，均摊复杂度接近 $O(1)$，足以应对本题的数据规模。
