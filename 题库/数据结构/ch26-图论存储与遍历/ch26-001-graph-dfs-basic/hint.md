```cpp
vector<int> g[100005];
bool visited[100005];
vector<int> order_;

void dfs(int u) {
    visited[u] = true;
    order_.push_back(u);
    for (int v : g[u]) {
        if (!visited[v]) dfs(v);
    }
}
```

**要点**：为了保证"优先访问编号最小的邻居"，需要在建图之后把每个点的邻接表 `sort` 一遍（`for (int i=1;i<=n;i++) sort(g[i].begin(), g[i].end());`），否则访问顺序会依赖输入边的给出顺序，而不是编号大小。

无向图的一条边 `u v` 要同时加进 `g[u]` 和 `g[v]` 两个方向的邻接表里，别只加一边。

$n$ 较大时如果图退化成一条长链，递归深度可能很深；本题数据已经控制图的结构不会出现这种极端情况，普通递归 DFS 可以安全通过。
