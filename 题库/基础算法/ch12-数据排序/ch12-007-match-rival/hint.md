排序后，与 x 最接近的候选只可能是两个：**第一个 ≥ x 的**（`lower_bound` 找到）和**它左边那个**。

```cpp
sort(b, b + n);
int p = lower_bound(b, b + n, x) - b;   // 第一个 >= x 的下标
long long ans = 4e18;
if (p < n)  ans = min(ans, (long long)b[p] - x);   // 右侧最近
if (p > 0)  ans = min(ans, (long long)x - b[p-1]); // 左侧最近
```

两个边界：x 比所有人都大（p == n，只有左边）、比所有人都小（p == 0，只有右边）。

复杂度：排序 O(n log n) + 每次查询 O(log n)，总计约 10^5 × 17 次比较——对比朴素 O(nm) = 10^10，这就是"**有序带来二分**"的威力。
