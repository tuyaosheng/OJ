```cpp
const long long INF = 1e18;
long long dist_[2005];
bool visited[2005];
vector<pair<int,int>> g[2005];  // g[u] = {(v, w), ...}

void dijkstra(int s, int n) {
    for (int i = 1; i <= n; i++) dist_[i] = INF;
    dist_[s] = 0;
    for (int iter = 0; iter < n; iter++) {
        int u = -1;
        for (int i = 1; i <= n; i++) {
            if (!visited[i] && (u == -1 || dist_[i] < dist_[u])) u = i;
        }
        if (u == -1 || dist_[u] == INF) break;  // 剩下的点都不可达，提前结束
        visited[u] = true;
        for (auto& [v, w] : g[u]) {
            if (dist_[u] + w < dist_[v]) dist_[v] = dist_[u] + w;
        }
    }
}
```

用 `INF`（一个足够大的数）表示"暂时不可达"，最后输出时如果 `dist[i]` 仍然是 `INF` 就输出 `-1`。

注意 Dijkstra 的正确性依赖"所有边权非负"——一旦选中 `u` 作为"已确定"，就默认它的 `dist` 不会再变小，这要求不存在"绕一圈反而更短"的负权边。如果图上有负权边，Dijkstra 会给出错误答案，这时需要用本章后面例4会学到的 Bellman-Ford / SPFA。
