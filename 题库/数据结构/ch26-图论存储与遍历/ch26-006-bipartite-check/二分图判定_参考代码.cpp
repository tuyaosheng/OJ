// 本题用到 vector、queue、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <queue> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
vector<int> g[MAXN];
int color_[MAXN];

bool bfsCheck(int s) {
    queue<int> q;
    q.push(s);
    color_[s] = 1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (color_[v] == 0) {
                color_[v] = 3 - color_[u];
                q.push(v);
            } else if (color_[v] == color_[u]) {
                return false;
            }
        }
    }
    return true;
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

    bool ok = true;
    for (int i = 1; i <= n && ok; i++) {
        if (color_[i] == 0) {
            if (!bfsCheck(i)) ok = false;
        }
    }
    cout << (ok ? "Yes" : "No") << '\n';
    return 0;
}
