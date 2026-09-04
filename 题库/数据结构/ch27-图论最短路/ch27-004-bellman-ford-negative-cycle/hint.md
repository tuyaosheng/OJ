```cpp
const long long INF = 1e18;
long long dist_[2005];
struct Edge { int u, v, w; };
vector<Edge> edges;

// 初始化
for (int i = 1; i <= n; i++) dist_[i] = INF;
dist_[s] = 0;

bool hasNegativeCycle = false;
for (int round = 1; round <= n; round++) {   // 多跑一轮（第 n 轮）用来检测负环
    bool updated = false;
    for (auto& e : edges) {
        if (dist_[e.u] == INF) continue;      // u 本身还不可达，这条边不能用来松弛
        if (dist_[e.u] + e.w < dist_[e.v]) {
            dist_[e.v] = dist_[e.u] + e.w;
            updated = true;
            if (round == n) { hasNegativeCycle = true; break; }
        }
    }
    if (!updated) break;  // 提前收敛，没必要跑满 n 轮
    if (hasNegativeCycle) break;
}
```

**关键易错点**：松弛一条边 `u v w` 之前，必须先检查 `dist[u]` 是不是还是初始的 `INF`（表示 $u$ 目前还不可达）。如果不做这个检查，当 $w$ 是负数时，`INF + w` 会是一个比 `INF` 略小的数，可能会被错误地当成"更优"更新到 `dist[v]` 上，把一个原本不可达的点误判成可达——这是新手在处理带负权边的最短路时最容易踩的坑。

第 $n$ 轮如果还能成功松弛，说明存在从 $s$ 可达的负环，直接输出 `NEGATIVE CYCLE` 并结束，不需要再继续跑。
