#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <queue>     —— priority_queue
//   <vector>    —— vector（priority_queue 的底层容器）
//   <functional> —— greater
// 考点：贪心 + 优先队列（小根堆）
// 结论：每次合并当前最小的两堆，代价最低

int main() {
    int n;
    cin >> n;
    priority_queue<long long, vector<long long>, greater<long long>> pq;  // 小根堆
    for (int i = 0; i < n; i++) {
        long long x;
        cin >> x;
        pq.push(x);
    }

    long long total = 0;
    while (pq.size() > 1) {
        long long x = pq.top(); pq.pop();   // 最小的一堆
        long long y = pq.top(); pq.pop();   // 第二小的一堆
        total += x + y;                     // 本次合并耗费
        pq.push(x + y);                     // 合并后的新堆放回去
    }
    cout << total << "\n";
    return 0;
}
