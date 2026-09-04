最容易出错的地方是**忘记声明小根堆**——直接写 `priority_queue<int> pq;` 得到的是大根堆，`top()` 会是最大值而不是最小值。一定要写全三个模板参数：

```cpp
priority_queue<int, vector<int>, greater<int>> pq;
```

处理 `n` 达到 $3\times10^5$ 级别的数据时，务必加快读写（`ios::sync_with_stdio(false); cin.tie(nullptr);`），否则纯 `cin/cout` 读入大量数据可能会超时。
