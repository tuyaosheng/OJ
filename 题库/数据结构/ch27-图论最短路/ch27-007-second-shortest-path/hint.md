```cpp
typedef pair<long long,int> pli;
const long long INF = 1e18;
long long dist1[10005], dist2[10005];

void solve(int s) {
    for (int i = 1; i <= n; i++) dist1[i] = dist2[i] = INF;
    dist1[s] = 0;
    priority_queue<pli, vector<pli>, greater<pli>> pq;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist2[u]) continue;   // 比这个点已知的最短、次短都差，没用了
        for (auto& [v, w] : g[u]) {
            long long nd = d + w;
            if (nd < dist1[v]) {
                swap(nd, dist1[v]);       // dist1[v] 变成更小的 nd，同时 nd 变成"被替换下来"的旧 dist1[v]
                pq.push({dist1[v], v});
            }
            if (nd > dist1[v] && nd < dist2[v]) {   // "被替换下来的值"如果严格大于新的最短、且比已知次短更小，就是新的次短候选
                dist2[v] = nd;
                pq.push({dist2[v], v});
            }
        }
    }
}
```

`swap(nd, dist1[v])` 这一步是本题最巧妙的地方：先把 `nd`（新算出的更优距离）和 `dist1[v]`（原来的最短距离）交换，交换后 `dist1[v]` 变成了更小的新值，而 `nd` 变成了"原来的最短距离"——这个"原来的最短距离"现在有资格去竞争"次短"这个位置（第二个 `if` 判断），这样一次遍历就同时处理了"最短被刷新"和"旧最短沦为次短候选"两件事。
