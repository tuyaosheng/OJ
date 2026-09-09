// 本题只用到 cin/cout，若不使用万能头，需要 #include <cstdio> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
int fa[MAXN];

int find(int x) {
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}

void merge(int x, int y) {
    fa[find(x)] = find(y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) fa[i] = i;
    for (int i = 0; i < m; i++) {
        int op, u, v;
        cin >> op >> u >> v;
        if (op == 1) {
            merge(u, v);
        } else {
            cout << (find(u) == find(v) ? "Yes" : "No") << '\n';
        }
    }
    return 0;
}
