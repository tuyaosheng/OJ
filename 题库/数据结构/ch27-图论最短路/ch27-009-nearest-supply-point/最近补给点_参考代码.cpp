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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, k;
    cin >> n >> m >> k;
    vector<int> supply(k);
    for (int i = 0; i < k; i++) cin >> supply[i];
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    for (int i = 1; i <= n; i++) dist_[i] = INF;
    priority_queue<pli, vector<pli>, greater<pli>> pq;
    for (int p : supply) {
        dist_[p] = 0;
        pq.push({0, p});
    }

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

    for (int i = 1; i <= n; i++) {
        if (i > 1) cout << ' ';
        cout << (dist_[i] == INF ? -1 : dist_[i]);
    }
    cout << '\n';
    return 0;
}
