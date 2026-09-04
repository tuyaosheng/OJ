用一个 `color` 数组代替 `visited`，`0` 表示未染色，`1`/`2` 表示两种颜色：

```cpp
int color[100005];  // 0 = 未染色

bool bfsCheck(int s) {
    queue<int> q;
    q.push(s);
    color[s] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (color[v] == 0) {
                color[v] = 3 - color[u];  // 染成和 u 相反的颜色（1 和 2 互换的小技巧）
                q.push(v);
            } else if (color[v] == color[u]) {
                return false;  // 邻居颜色和自己相同，矛盾
            }
        }
    }
    return true;
}

bool ok = true;
for (int i = 1; i <= n; i++) {
    if (color[i] == 0) {
        if (!bfsCheck(i)) { ok = false; break; }
    }
}
```

`3 - color[u]` 是"颜色 1 和 2 互相切换"的常用写法（`3-1=2`，`3-2=1`），比写 `if-else` 更简洁。

注意重边和自环（题目保证没有自环）：如果 $u,v$ 之间有重边，按上面的逻辑，第二次处理这条边时 `v` 已经染色且颜色和 `u` 相反，不会误判为矛盾，逻辑仍然正确。
