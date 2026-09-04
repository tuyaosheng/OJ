// 本题用到 vector、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <iostream>
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;
const int MAXN = 2005;
long long dist_[MAXN];
struct Edge { int u, v, w; };
vector<Edge> edges;
int n;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, s;
    cin >> n >> m >> s;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    for (int i = 1; i <= n; i++) dist_[i] = INF;
    dist_[s] = 0;

    bool hasNegativeCycle = false;
    for (int round = 1; round <= n; round++) {
        bool updated = false;
        for (auto& e : edges) {
            if (dist_[e.u] == INF) continue;
            if (dist_[e.u] + e.w < dist_[e.v]) {
                dist_[e.v] = dist_[e.u] + e.w;
                updated = true;
                if (round == n) { hasNegativeCycle = true; break; }
            }
        }
        if (hasNegativeCycle || !updated) break;
    }

    if (hasNegativeCycle) {
        cout << "NEGATIVE CYCLE\n";
    } else {
        for (int i = 1; i <= n; i++) {
            if (i > 1) cout << ' ';
            if (dist_[i] == INF) cout << "INF";
            else cout << dist_[i];
        }
        cout << '\n';
    }
    return 0;
}
