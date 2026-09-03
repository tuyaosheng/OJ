#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— min
typedef long long ll;
const ll INF = 4e18;

const int MAXN = 1005;
int a[MAXN][MAXN];
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
            dp[i][j] = INF;   // 先全部标记成“不可达”

    dp[1][1] = a[1][1];   // 边界（题目保证起点不是障碍物）
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (i == 1 && j == 1) continue;
            if (a[i][j] == -1) continue;   // 障碍物，不设置 dp 值
            ll best = min(dp[i - 1][j], dp[i][j - 1]);
            if (best < INF) dp[i][j] = a[i][j] + best;
        }
    }

    cout << (dp[n][m] < INF ? dp[n][m] : -1) << "\n";
    return 0;
}
