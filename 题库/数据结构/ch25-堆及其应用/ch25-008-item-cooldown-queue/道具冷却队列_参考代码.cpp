// 本题用到 queue（priority_queue）、utility（pair）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <queue> <utility> <iostream>
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            int t;
            cin >> t;
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
    return 0;
}
