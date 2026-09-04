// 本题用到 vector、utility（pair）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <utility> <iostream>
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;
const int MAXN = 2005;
long long dist_[MAXN];
bool visited_[MAXN];
vector<pair<int, int>> g[MAXN];
int n;

void dijkstra(int s) {
    for (int i = 1; i <= n; i++) dist_[i] = INF;
    dist_[s] = 0;
    for (int iter = 0; iter < n; iter++) {
        int u = -1;
        for (int i = 1; i <= n; i++) {
            if (!visited_[i] && (u == -1 || dist_[i] < dist_[u])) u = i;
        }
        if (u == -1 || dist_[u] == INF) break;
        visited_[u] = true;
        for (auto& pr : g[u]) {
            int v = pr.first, w = pr.second;
            if (dist_[u] + w < dist_[v]) dist_[v] = dist_[u] + w;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, s;
    cin >> n >> m >> s;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
    }
    dijkstra(s);
    for (int i = 1; i <= n; i++) {
        if (i > 1) cout << ' ';
        cout << (dist_[i] == INF ? -1 : dist_[i]);
    }
    cout << '\n';
    return 0;
}
