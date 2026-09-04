// 本题用到 vector、queue、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <queue> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MOD = 100003;
const int MAXN = 100005;
vector<int> g[MAXN];
int dist_[MAXN], cnt_[MAXN];
int n;

void bfs(int s) {
    for (int i = 1; i <= n; i++) dist_[i] = -1;
    dist_[s] = 0;
    cnt_[s] = 1;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (dist_[v] == -1) {
                dist_[v] = dist_[u] + 1;
                cnt_[v] = cnt_[u];
                q.push(v);
            } else if (dist_[v] == dist_[u] + 1) {
                cnt_[v] = (cnt_[v] + cnt_[u]) % MOD;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, s;
    cin >> n >> m >> s;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    bfs(s);
    for (int i = 1; i <= n; i++) {
        if (i > 1) cout << ' ';
        cout << (dist_[i] == -1 ? 0 : cnt_[i]);
    }
    cout << '\n';
    return 0;
}
