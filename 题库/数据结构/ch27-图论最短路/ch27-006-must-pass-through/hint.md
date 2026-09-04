直接复用例2堆优化 Dijkstra 的函数，调用两次即可：

```cpp
dijkstra(s);                 // 结果存进 distS[]
long long a = distS[k];

dijkstra(k);                 // 结果存进 distK[]（覆盖或用另一个数组存）
long long b = distK[t];

if (a == INF || b == INF) cout << -1 << '\n';
else cout << a + b << '\n';
```

注意两次调用 `dijkstra` 之间，`dist` 数组、`visited`（如果朴素版用到）等状态需要**重新初始化**——直接复用例2代码里"函数一开始就把 `dist` 全部重置为 `INF`"这一步，天然就处理好了这个问题。

$s = k$ 或 $k = t$ 的情况不需要特判：$s$ 到自己的最短距离天然是 $0$，加法自动得到正确结果。
