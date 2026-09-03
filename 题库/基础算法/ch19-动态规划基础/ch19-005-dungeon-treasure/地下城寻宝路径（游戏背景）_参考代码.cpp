#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— max
typedef long long ll;
const ll NEG_INF = -4e18;

const int MAXN = 1005;
ll a[MAXN][MAXN];
ll dp[MAXN][MAXN];
int n, m;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            cin >> a[i][j];

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= m; j++)
            dp[i][j] = NEG_INF;   // 先全部标记成“走不到”

    dp[1][1] = a[1][1];   // 边界
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (i == 1 && j == 1) continue;
            ll best = max(dp[i - 1][j], dp[i][j - 1]);
            dp[i][j] = a[i][j] + best;
        }
    }

    cout << dp[n][m] << "\n";
    return 0;
}
