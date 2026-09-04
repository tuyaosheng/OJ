往堆里存 `pair<int, int>`，第一个字段是冷却结束时刻，第二个字段是编号。C++ 的 `pair` 默认比较规则是"先比第一个字段，第一个字段相等再比第二个字段"，正好是题目要求的"先按结束时刻排，时刻相同按编号排"，不需要额外写比较函数：

```cpp
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
// pq 里的 pair 是 (结束时刻, 编号)

int cnt = 0;  // 记录已经分配了多少个编号
for (...) {
    if (op == 1) {
        int t; cin >> t;
        pq.push({t, ++cnt});
    } else {
        if (pq.empty()) {
            cout << -1 << ' ' << -1 << '\n';
        } else {
            auto [t, id] = pq.top();
            pq.pop();
            cout << id << ' ' << t << '\n';
        }
    }
}
```

注意输出顺序是"编号在前、结束时刻在后"，和 `pair` 内部存储的字段顺序（时刻在前、编号在后）是反过来的，别搞混了。
