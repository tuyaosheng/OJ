// 本题用到 queue（priority_queue）、vector、tuple、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <queue> <vector> <tuple> <iostream>
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int K;
    cin >> K;
    vector<vector<int>> arr(K);
    for (int i = 0; i < K; i++) {
        int c;
        cin >> c;
        arr[i].resize(c);
        for (int j = 0; j < c; j++) cin >> arr[i][j];
    }

    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    for (int i = 0; i < K; i++) {
        if (!arr[i].empty()) pq.push({arr[i][0], i, 0});
    }

    vector<int> ans;
    while (!pq.empty()) {
        auto [val, i, pos] = pq.top();
        pq.pop();
        ans.push_back(val);
        if (pos + 1 < (int)arr[i].size()) {
            pq.push({arr[i][pos + 1], i, pos + 1});
        }
    }

    for (size_t i = 0; i < ans.size(); i++) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}
