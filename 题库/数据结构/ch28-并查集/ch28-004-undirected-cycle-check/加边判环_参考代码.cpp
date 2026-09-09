// 本题只用到 cin/cout，若不使用万能头，需要 #include <cstdio> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
int fa[MAXN];

int find(int x) {
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) fa[i] = i;

    int firstCycleEdge = -1;
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        int ru = find(u), rv = find(v);
        if (ru == rv) {
            if (firstCycleEdge == -1) firstCycleEdge = i;
            continue;
        }
        fa[ru] = rv;
    }

    if (firstCycleEdge == -1) {
        cout << "No\n";
    } else {
        cout << "Yes\n" << firstCycleEdge << '\n';
    }
    return 0;
}
