```cpp
priority_queue<long long, vector<long long>, greater<long long>> pq;
for (int i = 0; i < n; i++) { long long w; cin >> w; pq.push(w); }

long long total = 0;
while (pq.size() > 1) {
    long long a = pq.top(); pq.pop();
    long long b = pq.top(); pq.pop();
    total += a + b;
    pq.push(a + b);
}
cout << total << '\n';
```

注意总代价可能远超过 `int` 能表示的范围（$n$ 堆、每堆最多 $10^4$，多轮合并累加下来可能到 $10^9$ 甚至更大），务必全程使用 `long long`，包括堆里存的元素类型。

$n=1$ 时循环一次都不会执行，`total` 保持 `0`，正好符合"不需要合并"的语义，不需要特判。
