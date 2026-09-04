```cpp
typedef pair<long long,int> pli;  // (距离, 点编号)
priority_queue<pli, vector<pli>, greater<pli>> pq;  // 小根堆

void dijkstra(int s, int n) {
    for (int i = 1; i <= n; i++) dist_[i] = INF;
    dist_[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist_[u]) continue;  // 过时的记录，跳过
        for (auto& [v, w] : g[u]) {
            if (dist_[u] + w < dist_[v]) {
                dist_[v] = dist_[u] + w;
                pq.push({dist_[v], v});
            }
        }
    }
}
```

**关键点**：这里不需要额外的 `visited` 数组，靠 `if (d > dist_[u]) continue;` 就能识别并跳过堆里"过时"的记录——因为同一个点可能因为多次被松弛而被压入堆多次，只有"弹出时的距离等于当前记录的最短距离"这一份才是有效的，其余的都是已经过时、不需要再处理的旧数据。

这正是"用堆维护动态候选集合、及时丢弃过时数据"这一技巧的直接应用，和"堆及其应用"那一章学到的思路一脉相承。
