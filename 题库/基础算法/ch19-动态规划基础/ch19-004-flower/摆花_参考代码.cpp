#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— min

typedef long long ll;
const ll MOD = 1000000007;

const int MAXN = 105;
int a[MAXN];
ll dp[MAXN][MAXN];
int n, m;

int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> a[i];

    dp[0][0] = 1;   // 边界：前 0 种花，摆 0 株
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            for (int k = 0; k <= min(a[i], j); k++) {
                dp[i][j] = (dp[i][j] + dp[i - 1][j - k]) % MOD;
            }
        }
    }

    cout << dp[n][m] << "\n";
    return 0;
}
