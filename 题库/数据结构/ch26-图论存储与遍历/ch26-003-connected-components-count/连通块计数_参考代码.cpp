// 本题用到 vector、queue、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <queue> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
vector<int> g[MAXN];
bool visited_[MAXN];

void bfs(int s) {
    queue<int> q;
    q.push(s);
    visited_[s] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
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
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int cnt = 0;
    for (int i = 1; i <= n; i++) {
        if (!visited_[i]) {
            cnt++;
            bfs(i);
        }
    }
    cout << cnt << '\n';
    return 0;
}
