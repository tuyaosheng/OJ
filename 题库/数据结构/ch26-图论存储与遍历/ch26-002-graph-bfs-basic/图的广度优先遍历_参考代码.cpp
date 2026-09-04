// 本题用到 vector、queue、algorithm（sort）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <queue> <algorithm> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
vector<int> g[MAXN];
bool visited_[MAXN];
vector<int> order_;

void bfs(int s) {
    queue<int> q;
    q.push(s);
    visited_[s] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order_.push_back(u);
        for (int v : g[u]) {
            if (!visited_[v]) {
                visited_[v] = true;
                q.push(v);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, s;
    cin >> n >> m >> s;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) sort(g[i].begin(), g[i].end());

    bfs(s);

    for (size_t i = 0; i < order_.size(); i++) {
        if (i) cout << ' ';
        cout << order_[i];
    }
    cout << '\n';
    return 0;
}
