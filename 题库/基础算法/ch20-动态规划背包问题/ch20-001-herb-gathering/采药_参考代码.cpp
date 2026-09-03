#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— max

const int MAXT = 1005;
int dp[MAXT];
int T, n;

int main() {
    cin >> T >> n;
    for (int i = 1; i <= n; i++) {
        int t, v;
        cin >> t >> v;
        for (int j = T; j >= t; j--) {           // 一维滚动数组：j 必须从大到小
            dp[j] = max(dp[j], dp[j - t] + v);
        }
    }
    cout << dp[T] << "\n";
    return 0;
}
