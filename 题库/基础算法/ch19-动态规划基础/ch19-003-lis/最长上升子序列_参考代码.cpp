#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— max

const int MAXN = 5005;
long long a[MAXN];
int dp[MAXN];
int n;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        dp[i] = 1;   // 边界：只选它自己
        for (int j = 1; j < i; j++) {
            if (a[j] < a[i]) dp[i] = max(dp[i], dp[j] + 1);
        }
        ans = max(ans, dp[i]);
    }

    cout << ans << "\n";
    return 0;
}
