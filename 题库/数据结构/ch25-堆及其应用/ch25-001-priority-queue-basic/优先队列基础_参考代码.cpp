// 本题用到 queue（priority_queue）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <queue> <iostream>
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < n; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            int x;
            cin >> x;
            pq.push(x);
        } else {
            if (pq.empty()) {
                cout << -1 << '\n';
            } else {
                cout << pq.top() << '\n';
                pq.pop();
            }
        }
    }
    return 0;
}
