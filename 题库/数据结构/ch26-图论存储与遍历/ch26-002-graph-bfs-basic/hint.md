```cpp
vector<int> g[100005];
bool visited[100005];
vector<int> order_;

void bfs(int s) {
    queue<int> q;
    q.push(s);
    visited[s] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order_.push_back(u);
        for (int v : g[u]) {
            if (!visited[v]) {
                visited[v] = true;   // 入队时就标记，不要等出队才标记
                q.push(v);
            }
        }
    }
}
```

和例1一样，先把每个点的邻接表排序，才能保证"优先访问编号小的邻居"。

**关键易错点**：`visited` 一定要在**入队的那一刻**就标记，而不是等这个点从队列里被取出来处理时才标记——如果拖到出队才标记，同一个点可能会因为被多个邻居同时发现而重复入队好几次，既浪费时间，也可能让访问顺序出错。
