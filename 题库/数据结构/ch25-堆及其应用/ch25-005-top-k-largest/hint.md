```cpp
priority_queue<int, vector<int>, greater<int>> pq;  // 小根堆，大小恒为 K
for (int i = 0; i < n; i++) {
    int x; cin >> x;
    if ((int)pq.size() < k) {
        pq.push(x);
    } else if (x > pq.top()) {
        pq.pop();
        pq.push(x);
    }
}
vector<int> ans;
while (!pq.empty()) { ans.push_back(pq.top()); pq.pop(); }
reverse(ans.begin(), ans.end());  // 堆里弹出的顺序是从小到大，要反过来变成从大到小
```

注意堆里最后弹出的顺序是"从小到大"（小根堆每次弹出的都是当前最小值），题目要求"从大到小"输出，记得反转一下结果数组，或者用 `vector` 存下来倒序遍历。
