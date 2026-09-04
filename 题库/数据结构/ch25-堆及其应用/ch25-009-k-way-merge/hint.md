堆里存 `tuple<int, int, int>`：`(元素值, 数组编号, 数组内下标)`。`tuple` 的默认比较规则和 `pair` 一样是"依次比较每个字段"，所以直接用 `greater<>` 建小根堆，天然按元素值排序（值相同时按数组编号、下标决定顺序，不影响正确性）。

```cpp
vector<vector<int>> arr(K);   // arr[i] 是第 i 个数组
priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<>> pq;

// 初始化：把每个数组的第一个元素放进堆
for (int i = 0; i < K; i++) {
    if (!arr[i].empty()) pq.push({arr[i][0], i, 0});
}

vector<int> ans;
while (!pq.empty()) {
    auto [val, i, pos] = pq.top();
    pq.pop();
    ans.push_back(val);
    if (pos + 1 < (int)arr[i].size()) {
        pq.push({arr[i][pos + 1], i, pos + 1});
    }
}
```

堆里同一时刻最多有 $K$ 个元素（每个数组最多贡献一个"当前候选"），每个元素总共只会入堆、出堆各一次，整体复杂度是 $O(N \log K)$，比"先把所有数丢进一个大小为 $N$ 的堆"（$O(N \log N)$）更快，尤其是 $K$ 远小于 $N$ 的时候。
