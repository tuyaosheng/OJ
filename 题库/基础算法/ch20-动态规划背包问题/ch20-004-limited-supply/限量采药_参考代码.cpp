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
        int t, v, c;      // t=耗时，v=价值，c=这种草药最多采几株（多重背包）
        cin >> t >> v >> c;
        for (int j = T; j >= t; j--) {
            int maxK = min(c, j / t);   // 受限于库存 c，也受限于剩余时间 j 最多能采几株
            for (int k = 1; k <= maxK; k++) {
                dp[j] = max(dp[j], dp[j - k * t] + k * v);   // 枚举这种草药实际采了 k 株
            }
        }
    }
    cout << dp[T] << "\n";
    return 0;
}
