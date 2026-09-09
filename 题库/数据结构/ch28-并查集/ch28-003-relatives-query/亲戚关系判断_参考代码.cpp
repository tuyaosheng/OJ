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
    int n, M, Q;
    cin >> n >> M >> Q;
    for (int i = 1; i <= n; i++) fa[i] = i;
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        merge(u, v);
    }
    for (int i = 0; i < Q; i++) {
        int a, b;
        cin >> a >> b;
        cout << (find(a) == find(b) ? "Yes" : "No") << '\n';
    }
    return 0;
}
