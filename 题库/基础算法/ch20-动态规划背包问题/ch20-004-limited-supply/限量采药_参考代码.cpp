#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— max、min

const int MAXT = 1005;
int dp[MAXT];
int T, n;

int main() {
    cin >> T >> n;
    for (int i = 1; i <= n; i++) {
        int t, v, c;
        cin >> t >> v >> c;
        for (int j = T; j >= t; j--) {
            int maxK = min(c, j / t);
            for (int k = 1; k <= maxK; k++) {
                dp[j] = max(dp[j], dp[j - k * t] + k * v);
            }
        }
    }
    cout << dp[T] << "\n";
    return 0;
}
