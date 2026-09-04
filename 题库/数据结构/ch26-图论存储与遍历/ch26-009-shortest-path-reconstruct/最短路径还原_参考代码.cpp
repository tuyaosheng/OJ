// 本题用到 vector、queue、algorithm（sort/reverse）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <queue> <algorithm> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
vector<int> g[MAXN];
int dist_[MAXN], prev_[MAXN];
int n;

void bfs(int s) {
    for (int i = 1; i <= n; i++) dist_[i] = -1;
    queue<int> q;
    q.push(s);
    dist_[s] = 0;
    prev_[s] = -1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (dist_[v] == -1) {
                dist_[v] = dist_[u] + 1;
                prev_[v] = u;
                q.push(v);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, s, t;
    cin >> n >> m >> s >> t;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) sort(g[i].begin(), g[i].end());

    bfs(s);

    if (dist_[t] == -1) {
        cout << -1 << '\n';
    } else {
        vector<int> path;
        for (int cur = t; cur != -1; cur = prev_[cur]) path.push_back(cur);
        reverse(path.begin(), path.end());
        cout << dist_[t] << '\n';
        for (size_t i = 0; i < path.size(); i++) {
            if (i) cout << ' ';
            cout << path[i];
        }
        cout << '\n';
    }
    return 0;
}
