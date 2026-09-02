**贪心结论：每次合并当前最小的两堆。**

直觉：越早合并的堆，它的重量会在后面的合并中被**重复计算**的次数越多（就像利息一样滚）。所以应该让最小的两堆尽早合并、尽早"退场"，避免它们被反复计入后续的耗费。

如何高效地"每次找出最小的两堆"？——**小根堆（优先队列）**：

```cpp
priority_queue<int, vector<int>, greater<int>> pq;  // 小根堆
for (每堆果子) pq.push(a[i]);
long long total = 0;
while (pq.size() > 1) {
    int x = pq.top(); pq.pop();
    int y = pq.top(); pq.pop();
    total += x + y;         // 这次合并的耗费
    pq.push(x + y);         // 新堆放回去，继续参与后面的合并
}
```

n=10000、每堆最多 20000，总重量上限 2×10⁸，合并过程近似哈夫曼树，总耗费可能到 10⁹ 级——**用 long long**。
