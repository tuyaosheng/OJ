#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>    —— cin / cout
//   <queue>       —— priority_queue
//   <vector>      —— vector（priority_queue 的底层容器）
//   <functional>  —— greater

int main()
{
    int n;
    cin >> n;
    // 小根堆：最小的元素排在堆顶
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        pq.push(x);
    }
    long long ans = 0;
    // 每次取最小的两堆合并，直到只剩一堆
    while (pq.size() > 1)
    {
        int a = pq.top(); pq.pop();
        int b = pq.top(); pq.pop();
        ans += a + b;                // 本次合并花费
        pq.push(a + b);              // 合并后的新堆放回
    }
    cout << ans << "\n";
    return 0;
}
