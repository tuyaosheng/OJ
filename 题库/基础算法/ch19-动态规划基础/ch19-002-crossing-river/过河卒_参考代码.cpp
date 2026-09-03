#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
typedef long long ll;

const int MAXN = 25;
bool blocked[MAXN][MAXN];
ll dp[MAXN][MAXN];
int n, m, bx, by;
int hx[9] = {0, -1, -1, 1, 1, -2, -2, 2, 2};
int hy[9] = {0, -2, 2, -2, 2, -1, 1, -1, 1};

int main() {
    cin >> n >> m >> bx >> by;

    for (int k = 0; k < 9; k++) {
        int x = bx + hx[k], y = by + hy[k];
        if (x >= 0 && x <= n && y >= 0 && y <= m) blocked[x][y] = true;
    }

    if (!blocked[0][0]) dp[0][0] = 1;   // 边界
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (i == 0 && j == 0) continue;
            if (blocked[i][j]) continue;
            ll fromUp = (i > 0) ? dp[i - 1][j] : 0;
            ll fromLeft = (j > 0) ? dp[i][j - 1] : 0;
            dp[i][j] = fromUp + fromLeft;
        }
    }

    cout << dp[n][m] << "\n";
    return 0;
}
