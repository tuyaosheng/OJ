// 本题用到 vector、algorithm（sort）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <algorithm> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
int fa[MAXN];

int find(int x) {
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}

struct Road {
    int u, v, t;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) fa[i] = i;

    vector<Road> roads(m);
    for (int i = 0; i < m; i++) cin >> roads[i].u >> roads[i].v >> roads[i].t;
    sort(roads.begin(), roads.end(), [](const Road& a, const Road& b) {
        return a.t < b.t;
    });

    int components = n;
    long long answer = -1;
    if (components == 1) answer = 0;
    for (auto& r : roads) {
        int ru = find(r.u), rv = find(r.v);
        if (ru != rv) {
            fa[ru] = rv;
            components--;
            if (components == 1) {
                answer = r.t;
                break;
            }
        }
    }
    cout << answer << '\n';
    return 0;
}
