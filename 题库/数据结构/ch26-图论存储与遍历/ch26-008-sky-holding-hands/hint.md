和例3"连通块计数"几乎一样，只是这次不只是数个数，还要记录**每个点具体属于哪个连通块**：

```cpp
int comp[100005];  // comp[i] = 0 表示未访问，否则是圈子编号（从 1 开始）

void bfs(int s, int cid) {
    queue<int> q;
    q.push(s);
    comp[s] = cid;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (comp[v] == 0) {
                comp[v] = cid;
                q.push(v);
            }
        }
    }
}

int cid = 0;
for (int i = 1; i <= n; i++) {
    if (comp[i] == 0) {
        cid++;
        bfs(i, cid);
    }
}

// 每次询问 O(1) 回答
for each query (a, b): cout << (comp[a] == comp[b] ? "Yes" : "No") << '\n';
```

预处理是 $O(n+m)$，之后每次询问只需要比较两个整数，$Q$ 次询问总共 $O(Q)$——如果不预处理、每次询问都现场 BFS，最坏情况 $O(Q \cdot (n+m))$ 会严重超时，这也是这道题和普通"判断两点是否连通"的区别所在。
