// 本题用到 vector、queue（priority_queue）、utility（pair）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <queue> <utility> <iostream>
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, int> pli;

const ll INF = 1e18;
const int MAXN = 100005;
vector<pair<int, int>> g[MAXN];
ll dist_[MAXN];
int n;

void dijkstra(int s) {
    for (int i = 1; i <= n; i++) dist_[i] = INF;
    dist_[s] = 0;
    priority_queue<pli, vector<pli>, greater<pli>> pq;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist_[u]) continue;
        for (auto& pr : g[u]) {
            int v = pr.first, w = pr.second;
            if (dist_[u] + w < dist_[v]) {
                dist_[v] = dist_[u] + w;
                pq.push({dist_[v], v});
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, s, t, k;
    cin >> n >> m >> s >> t >> k;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
    }

    dijkstra(s);
    ll a = dist_[k];

    dijkstra(k);
    ll b = dist_[t];

    if (a == INF || b == INF) cout << -1 << '\n';
    else cout << a + b << '\n';
    return 0;
}
