```cpp
vector<int> g[100005];
int dist_[100005], prev_[100005];

void bfs(int s) {
    for (int i = 1; i <= n; i++) dist_[i] = -1;
    queue<int> q;
    q.push(s);
    dist_[s] = 0;
    prev_[s] = -1;   // 起点没有前驱
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist_[v] == -1) {
                dist_[v] = dist_[u] + 1;
                prev_[v] = u;
                q.push(v);
            }
        }
    }
}
```

还原路径时，从 $t$ 往回走收集到一个 `vector`，最后 `reverse` 一下：

```cpp
if (dist_[t] == -1) {
    cout << -1 << '\n';
} else {
    vector<int> path;
    for (int cur = t; cur != -1; cur = prev_[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());
    cout << dist_[t] << '\n';
    // 依次输出 path 里的节点
}
```

记得先把邻接表排序（和例1、例2一样），否则 `prev` 数组不确定，还原出的路径可能和标准答案不一致。
