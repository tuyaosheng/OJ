#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
typedef long long ll;
const ll MOD = 1000000007;

const int MAXS = 1005;
ll dp[MAXS];
int n, S;

int main() {
    cin >> n >> S;
    dp[0] = 1;      // 凑出总额 0 只有"什么都不选"这一种方案
    for (int i = 1; i <= n; i++) {
        int w;
        cin >> w;
        // 完全背包（每种硬币可重复用）：j 从小到大递增遍历，
        // 这样统计的是"方案"（不区分取硬币的顺序），而不是排列
        for (int j = w; j <= S; j++) {
            dp[j] = (dp[j] + dp[j - w]) % MOD;
        }
    }
    cout << dp[S] << "\n";
    return 0;
}
