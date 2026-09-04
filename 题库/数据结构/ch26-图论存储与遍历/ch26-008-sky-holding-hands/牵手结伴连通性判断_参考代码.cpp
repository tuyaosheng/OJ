// 本题用到 vector、queue、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <queue> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
vector<int> g[MAXN];
int comp[MAXN];

void bfs(int s, int cid) {
    queue<int> q;
    q.push(s);
    comp[s] = cid;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (comp[v] == 0) {
                comp[v] = cid;
                q.push(v);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, Q;
    cin >> n >> m >> Q;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int cid = 0;
    for (int i = 1; i <= n; i++) {
        if (comp[i] == 0) {
            cid++;
            bfs(i, cid);
        }
    }

    for (int i = 0; i < Q; i++) {
        int a, b;
        cin >> a >> b;
        cout << (comp[a] == comp[b] ? "Yes" : "No") << '\n';
    }
    return 0;
}
