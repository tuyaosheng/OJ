**核心变形**：`a[i] - a[j] = C` 等价于 `a[i] = a[j] + C`。

所以：**枚举 j，然后数出数组中值等于 `a[j] + C` 的元素有多少个**，全部累加即为答案。

**怎么快速数出"等于某个值的元素有多少个"？**

排序之后，相同的数会连成一段。用两次二分就能算出这一段有多长：

- `lower_bound(a, a+n, x)` → 第一个 **≥ x** 的位置
- `upper_bound(a, a+n, x)` → 第一个 **> x** 的位置
- 两者之差，就是**等于 x 的元素个数**！

```cpp
sort(a, a + n);

long long ans = 0;

for (int j = 0; j < n; j++)
{
    long long target = (long long)a[j] + C;   // 防溢出：先转 long long

    int cnt = upper_bound(a, a + n, target) - lower_bound(a, a + n, target);
    ans += cnt;
}

cout << ans << "\n";
```

**几个关键的坑**：

1. **答案必须用 `long long`**。最坏情况（比如所有数相同、C 使得每个数都能配上）答案可达 n² = 10^10，远超 int 的 21 亿。
2. **`a[j] + C` 可能溢出 int**！a[j] 最大 10^9，C 最大 10^9，相加是 2 × 10^9 —— 超过 int 上限（约 2.147 × 10^9）？很接近，但确实可能超。**先转成 `long long` 再相加最保险。**
3. 由于 C ≥ 1（严格为正），`a[i] = a[j] + C` 保证了 `a[i] ≠ a[j]`，所以 **i 和 j 自然不会是同一个下标**，不必额外排除。

**复杂度**：排序 O(n log n) + n 次二分 O(n log n)，总体 **O(n log n)**。10 万级数据轻松通过。
