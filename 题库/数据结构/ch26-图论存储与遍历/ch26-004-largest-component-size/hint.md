让 `bfs` 函数返回这次遍历一共访问了多少个点即可：

```cpp
int bfs(int s) {
    queue<int> q;
    q.push(s);
    visited[s] = true;
    int cnt = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        cnt++;
        for (int v : g[u]) {
            if (!visited[v]) { visited[v] = true; q.push(v); }
        }
    }
    return cnt;
}

int ans = 0;
for (int i = 1; i <= n; i++) {
    if (!visited[i]) ans = max(ans, bfs(i));
}
```

和例3一样，别忘了孤立点（大小为 1 的连通块）也要参与"取最大值"的比较。
