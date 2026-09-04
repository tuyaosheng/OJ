不需要真的显式建出邻接表——网格图的"邻居"可以直接用坐标运算得到（四个方向的偏移量 `dx[] = {-1,1,0,0}`, `dy[] = {0,0,-1,1}`），BFS/DFS 时直接在四个方向上试探即可：

```cpp
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
bool visited[1005][1005];
char grid[1005][1005];

bool bfs(int sr, int sc, int tr, int tc) {
    queue<pair<int,int>> q;
    q.push({sr, sc});
    visited[sr][sc] = true;
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == tr && c == tc) return true;
        for (int d = 0; d < 4; d++) {
            int nr = r + dx[d], nc = c + dy[d];
            if (nr < 1 || nr > R || nc < 1 || nc > C) continue;  // 越界
            if (grid[nr][nc] == '#' || visited[nr][nc]) continue; // 墙或已访问
            visited[nr][nc] = true;
            q.push({nr, nc});
        }
    }
    return false;
}
```

$R, C$ 最大到 $1000$，网格总共最多 $10^6$ 个格子，BFS/DFS 复杂度 $O(RC)$ 完全够用；但如果用递归 DFS，网格較大时最坏情况下递归深度可能到 $10^6$，有栈溢出风险——本题更推荐用 BFS（显式队列，不受系统递归栈深度限制）。
