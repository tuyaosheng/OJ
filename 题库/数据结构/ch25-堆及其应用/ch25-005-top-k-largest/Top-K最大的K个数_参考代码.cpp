// 本题用到 queue（priority_queue）、vector、algorithm（reverse）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <queue> <vector> <algorithm> <iostream>
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    cin >> n >> k;
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        if ((int)pq.size() < k) {
            pq.push(x);
        } else if (x > pq.top()) {
            pq.pop();
            pq.push(x);
        }
    }
    vector<int> ans;
    while (!pq.empty()) {
        ans.push_back(pq.top());
        pq.pop();
    }
    reverse(ans.begin(), ans.end());
    for (size_t i = 0; i < ans.size(); i++) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}
