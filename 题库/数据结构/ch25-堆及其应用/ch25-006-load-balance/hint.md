```cpp
priority_queue<long long, vector<long long>, greater<long long>> pq;
for (int i = 0; i < m; i++) pq.push(0);  // m 台服务器初始负载都是 0

for (int i = 0; i < n; i++) {
    long long t; cin >> t;
    long long minLoad = pq.top(); pq.pop();
    pq.push(minLoad + t);
}

long long ans = 0;
while (!pq.empty()) { ans = max(ans, pq.top()); pq.pop(); }
cout << ans << '\n';
```

注意累计负载可能超过 `int` 范围（$n$ 最多 $2\times10^5$ 个任务、每个最多 $10^4$，极端情况下如果 $m=1$ 全部任务堆在一台服务器上，总和可以到 $2\times10^9$），务必用 `long long`。

$m$ 可能比 $n$ 大很多（服务器比任务还多），这时多出来的服务器负载始终是 $0$，代码不需要特殊处理——初始的 $m$ 个 $0$ 本来就已经在堆里参与比较了。
