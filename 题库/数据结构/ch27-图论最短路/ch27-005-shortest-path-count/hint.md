```cpp
const int MOD = 100003;
int dist_[100005], cnt_[100005];

void bfs(int s) {
    for (int i = 1; i <= n; i++) dist_[i] = -1;
    dist_[s] = 0;
    cnt_[s] = 1;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist_[v] == -1) {
                dist_[v] = dist_[u] + 1;
                cnt_[v] = cnt_[u];
                q.push(v);
            } else if (dist_[v] == dist_[u] + 1) {
                cnt_[v] = (cnt_[v] + cnt_[u]) % MOD;
            }
        }
    }
}
```

注意区分两种情况：`dist[v] == -1`（第一次到达，直接继承）和 `dist[v] == dist[u] + 1`（曾经到达过，且这次走法长度依然是最短，要累加而不是覆盖）——如果 `dist[v] < dist[u] + 1`，说明这条路径比已知的最短路更长，不是最短路的一部分，直接忽略即可。
