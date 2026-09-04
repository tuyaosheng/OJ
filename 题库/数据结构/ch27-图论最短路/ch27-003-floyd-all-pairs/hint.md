```cpp
const int INF = 0x3f3f3f3f;  // 一个足够大、又不容易在相加时溢出的数
int dist_[305][305];

for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++)
        dist_[i][j] = (i == j) ? 0 : INF;

for (每条边 u v w)
    dist_[u][v] = min(dist_[u][v], w);  // 重边取较小的

for (int k = 1; k <= n; k++)
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (dist_[i][k] + dist_[k][j] < dist_[i][j])
                dist_[i][j] = dist_[i][k] + dist_[k][j];
```

`INF` 不要用 `int` 能表示的最大值（比如 `INT_MAX`），否则 `dist[i][k] + dist[k][j]` 两个"不可达"的距离相加会**溢出**变成一个负数，错误地"更新"出一条不存在的更短路径。用 `0x3f3f3f3f`（约 $10^9$）是竞赛里的经典技巧——它足够大，同时两个这样的数相加也不会超过 `int` 的范围。

读入边时如果同一对 $(u,v)$ 出现多次，要取权值较小的那条，直接读入时 `dist[u][v] = min(dist[u][v], w)` 即可覆盖处理。
