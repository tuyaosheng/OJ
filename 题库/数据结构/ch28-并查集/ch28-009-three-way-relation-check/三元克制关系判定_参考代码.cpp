// 本题只用到 cin/cout，若不使用万能头，需要 #include <cstdio> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 50005;
int fa[MAXN], rel[MAXN];

int find(int x) {
    if (fa[x] == x) return x;
    int root = find(fa[x]);
    rel[x] = (rel[x] + rel[fa[x]]) % 3;
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
        int k, u, v;
        cin >> k >> u >> v;
        if (k == 2 && u == v) {
            falseCount++;
            continue;
        }
        int expected = (k == 1) ? 0 : 1;
        int ru = find(u), rv = find(v);
        if (ru == rv) {
            int actual = ((rel[u] - rel[v]) % 3 + 3) % 3;
            if (actual != expected) falseCount++;
        } else {
            fa[ru] = rv;
            rel[ru] = ((expected - rel[u] + rel[v]) % 3 + 3) % 3;
        }
    }
    cout << falseCount << '\n';
    return 0;
}
