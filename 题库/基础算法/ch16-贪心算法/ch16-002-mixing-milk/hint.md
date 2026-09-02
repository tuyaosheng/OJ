**贪心结论：先从最便宜的供应商买，买到不能再买（他卖完了或者你够了）为止。**

这次的证明更直接：如果你手里的钱要买同样多的牛奶，从便宜的地方买肯定比从贵的地方买省钱——**能省的钱，绝不多花**。所以先把最便宜的供应商买光，不够再买第二便宜的，以此类推。

```cpp
sort(supplier, supplier + m, cmp);   // 按单价从小到大排序
long long cost = 0, remain = need;
for (int i = 0; i < m && remain > 0; i++) {
    int buy = min((long long)supplier[i].amount, remain);  // 这家最多买这么多
    cost += (long long)buy * supplier[i].price;
    remain -= buy;
}
```

花费最大约 2×10⁶ × 1000 = 2×10⁹，**必须 long long**。
