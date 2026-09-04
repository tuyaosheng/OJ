// 本题用到 algorithm（min）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <algorithm> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int INF = 0x3f3f3f3f;
const int MAXN = 305;
int dist_[MAXN][MAXN];
int n;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, Q;
    cin >> n >> m >> Q;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            dist_[i][j] = (i == j) ? 0 : INF;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        dist_[u][v] = min(dist_[u][v], w);
    }

    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                if (dist_[i][k] + dist_[k][j] < dist_[i][j])
                    dist_[i][j] = dist_[i][k] + dist_[k][j];

    for (int i = 0; i < Q; i++) {
        int a, b;
        cin >> a >> b;
        cout << (dist_[a][b] >= INF ? -1 : dist_[a][b]) << '\n';
    }
    return 0;
}
