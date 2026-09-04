// 本题用到 vector、queue（priority_queue）、utility（pair）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <queue> <utility> <iostream>
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, int> pli;

const ll INF = 1e18;
const int MAXN = 10005;
vector<pair<int, int>> g[MAXN];
ll dist1[MAXN], dist2[MAXN];
int n;

void solve(int s) {
    for (int i = 1; i <= n; i++) dist1[i] = dist2[i] = INF;
    dist1[s] = 0;
    priority_queue<pli, vector<pli>, greater<pli>> pq;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist2[u]) continue;
        for (auto& pr : g[u]) {
            int v = pr.first, w = pr.second;
            ll nd = d + w;
            if (nd < dist1[v]) {
                swap(nd, dist1[v]);
                pq.push({dist1[v], v});
            }
            if (nd > dist1[v] && nd < dist2[v]) {
                dist2[v] = nd;
                pq.push({dist2[v], v});
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
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
    }
    solve(s);
    cout << (dist2[t] == INF ? -1 : dist2[t]) << '\n';
    return 0;
}
