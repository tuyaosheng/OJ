插入一个新数 `x` 时：

1. 先决定放进哪个堆：如果 `lower` 为空，或者 `x <= lower.top()`，就放进 `lower`（较小的一半）；否则放进 `upper`（较大的一半）。
2. 插入之后可能导致两个堆的大小不平衡，需要重新调整：
   - 如果 `lower` 比 `upper` 多了 $2$ 个及以上，把 `lower` 的堆顶挪到 `upper`。
   - 如果 `upper` 比 `lower` 还多，把 `upper` 的堆顶挪到 `lower`。
3. 调整完之后，`lower.top()` 就是当前的中位数。

```cpp
priority_queue<int> lower;                                   // 大根堆，较小的一半
priority_queue<int, vector<int>, greater<int>> upper;         // 小根堆，较大的一半

void insert(int x) {
    if (lower.empty() || x <= lower.top()) lower.push(x);
    else upper.push(x);

    if (lower.size() > upper.size() + 1) {
        upper.push(lower.top());
        lower.pop();
    } else if (upper.size() > lower.size()) {
        lower.push(upper.top());
        upper.pop();
    }
}
```

每次插入和查询中位数都是 $O(\log n)$，$n$ 次操作总共 $O(n \log n)$，可以轻松通过。
