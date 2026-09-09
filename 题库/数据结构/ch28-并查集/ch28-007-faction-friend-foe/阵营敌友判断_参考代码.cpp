// 本题只用到 cin/cout，若不使用万能头，需要 #include <cstdio> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
int fa[MAXN], rel[MAXN];

int find(int x) {
    if (fa[x] == x) return x;
    int root = find(fa[x]);
    rel[x] ^= rel[fa[x]];
    fa[x] = root;
    return root;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) { fa[i] = i; rel[i] = 0; }

    int falseCount = 0;
    for (int i = 0; i < m; i++) {
        int c, u, v;
        cin >> c >> u >> v;
        int ru = find(u), rv = find(v);
        if (ru == rv) {
            if ((rel[u] ^ rel[v]) != c) falseCount++;
        } else {
            fa[ru] = rv;
            rel[ru] = c ^ rel[u] ^ rel[v];
        }
    }
    cout << falseCount << '\n';
    return 0;
}
