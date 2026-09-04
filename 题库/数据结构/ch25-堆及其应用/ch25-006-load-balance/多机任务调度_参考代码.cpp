// 本题用到 queue（priority_queue）、algorithm（max）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <queue> <algorithm> <iostream>
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, n;
    cin >> m >> n;
    priority_queue<long long, vector<long long>, greater<long long>> pq;
    for (int i = 0; i < m; i++) pq.push(0);

    for (int i = 0; i < n; i++) {
        long long t;
        cin >> t;
        long long minLoad = pq.top();
        pq.pop();
        pq.push(minLoad + t);
    }

    long long ans = 0;
    while (!pq.empty()) {
        ans = max(ans, pq.top());
        pq.pop();
    }
    cout << ans << '\n';
    return 0;
}
