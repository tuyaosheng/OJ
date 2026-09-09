```cpp
int firstCycleEdge = -1;
for (int i = 1; i <= m; i++) {
    int u, v;
    cin >> u >> v;
    int ru = find(u), rv = find(v);
    if (ru == rv) {
        if (firstCycleEdge == -1) firstCycleEdge = i;  // 记录第一次出现环的位置
        continue;  // 已经连通，这条边不需要（也不应该）再合并
    }
    fa[ru] = rv;
}
if (firstCycleEdge == -1) cout << "No\n";
else cout << "Yes\n" << firstCycleEdge << '\n';
```

关键在于**判断顺序**：一定要**先查询**（`find(u) == find(v)`）**再决定要不要合并**。如果先合并了再判断，等于把这条"多余的边"也吃进了并查集里，虽然本题因为 `merge` 对同根的点合并是无效操作、不会直接导致后续判断出错，但这个"先判环、再合并"的顺序在下一章 Kruskal 算法里至关重要——Kruskal 需要严格跳过"会成环的边"，绝不能真的把它们合并进去。

记录"第一条成环的边"只需要一个标记变量，找到后不必清空或修改并查集状态，继续正常处理剩下的边即可（后面的边不影响"第一条"这个答案）。
