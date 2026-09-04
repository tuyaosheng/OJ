// 本题用到 vector、queue、algorithm（max）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <queue> <algorithm> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
vector<int> g[MAXN];
bool visited_[MAXN];

int bfs(int s) {
    queue<int> q;
    q.push(s);
    visited_[s] = true;
    int cnt = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        cnt++;
        for (int v : g[u]) {
            if (!visited_[v]) {
                visited_[v] = true;
                q.push(v);
            }
        }
    }
    return cnt;
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

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (!visited_[i]) ans = max(ans, bfs(i));
    }
    cout << ans << '\n';
    return 0;
}
