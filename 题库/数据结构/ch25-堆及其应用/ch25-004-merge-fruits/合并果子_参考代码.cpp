// 本题用到 queue（priority_queue）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <queue> <iostream>
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    priority_queue<long long, vector<long long>, greater<long long>> pq;
    for (int i = 0; i < n; i++) {
        long long w;
        cin >> w;
        pq.push(w);
    }

    long long total = 0;
    while (pq.size() > 1) {
        long long a = pq.top(); pq.pop();
        long long b = pq.top(); pq.pop();
        total += a + b;
        pq.push(a + b);
    }
    cout << total << '\n';
    return 0;
}
