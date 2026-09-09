```cpp
int fa[100005], rel[100005];  // rel[x]：x 相对于 fa[x] 的关系，0=同阵营，1=不同阵营

void init(int n) {
    for (int i = 1; i <= n; i++) { fa[i] = i; rel[i] = 0; }
}

int find(int x) {
    if (fa[x] == x) return x;
    int root = find(fa[x]);      // 先递归找到根（此时 fa[x] 的 rel 已经变成"相对于根"了）
    rel[x] ^= rel[fa[x]];        // 累加关系：x 相对于根 = x 相对于旧父亲 异或 旧父亲相对于根
    fa[x] = root;                // 路径压缩
    return root;
}

int falseCount = 0;
for (每条陈述 c u v) {
    int ru = find(u), rv = find(v);
    if (ru == rv) {
        // 已经知道 u、v 的关系，检查是否和这条陈述矛盾
        if ((rel[u] ^ rel[v]) != c) falseCount++;
    } else {
        // 合并，让新根 ru 相对于 rv 的关系满足：c = rel[u] ^ rel[ru] ^ rel[v]
        fa[ru] = rv;
        rel[ru] = c ^ rel[u] ^ rel[v];
    }
}
cout << falseCount << '\n';
```

`find` 函数是本题唯一的难点：递归先找到根，回溯时再更新 `rel[x]`——**顺序不能反**，必须先让 `fa[x]` 的 `rel` 值变成"相对于根"，才能正确地把它累加到 `x` 身上。合并时的公式 `rel[ru] = c ^ rel[u] ^ rel[v]` 是从"最终 $u, v$ 的关系必须等于 $c$"这个目标反推出来的，建议对照讲解稿里的推导过程理解，而不是死记硬背。
