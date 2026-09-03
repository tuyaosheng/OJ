#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— max
typedef long long ll;

const int MAXT = 100005;
ll dp[MAXT];
int T, n;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> T >> n;
    for (int i = 1; i <= n; i++) {
        int t, v;
        cin >> t >> v;
        for (int j = t; j <= T; j++) {           // 完全背包：j 从小到大
            dp[j] = max(dp[j], dp[j - t] + v);
        }
    }
    cout << dp[T] << "\n";
    return 0;
}
