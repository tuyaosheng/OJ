#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— max
typedef long long ll;

const int MAXN = 1005;
ll a[MAXN][MAXN];
ll dp[MAXN][MAXN];
int n;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= i; j++)
            cin >> a[i][j];

    for (int j = 1; j <= n; j++) dp[n][j] = a[n][j];   // 边界：最后一行

    for (int i = n - 1; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            dp[i][j] = a[i][j] + max(dp[i + 1][j], dp[i + 1][j + 1]);
        }
    }

    cout << dp[1][1] << "\n";
    return 0;
}
