// 本题用到 algorithm（swap）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <algorithm> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
int fa[MAXN], sz[MAXN];

int find(int x) {
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}

void merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) return;
    if (sz[x] > sz[y]) swap(x, y);
    fa[x] = y;
    sz[y] += sz[x];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) { fa[i] = i; sz[i] = 1; }
    for (int i = 0; i < m; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            int u, v;
            cin >> u >> v;
            merge(u, v);
        } else {
            int u;
            cin >> u;
            cout << sz[find(u)] << '\n';
        }
    }
    return 0;
}
