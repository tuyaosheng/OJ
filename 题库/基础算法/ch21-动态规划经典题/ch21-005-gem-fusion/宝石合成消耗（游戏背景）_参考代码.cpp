#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <cstring>  —— memset
//   <algorithm> —— min

const int MAXN = 205;
int a[MAXN];
long long prefix[MAXN];
long long dp[MAXN][MAXN];   // dp[i][j] = 把第 i~j 堆宝石合成一堆的最小消耗
int n;

int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        a[i + n] = a[i];   // 环形排列：复制一份接在后面，破环成链
    }
    int m = 2 * n;
    for (int i = 1; i <= m; i++) prefix[i] = prefix[i - 1] + a[i];

    memset(dp, 0, sizeof(dp));
    // 区间 DP：按长度从小到大枚举，dp[i][j] 由所有分割点 k 的最优值转移而来
    for (int len = 2; len <= n; len++) {
        for (int i = 1; i + len - 1 <= m; i++) {
            int j = i + len - 1;
            dp[i][j] = 1e18;
            long long segSum = prefix[j] - prefix[i - 1];   // 合并 [i,j] 这一次的消耗 = 区间总和
            for (int k = i; k < j; k++) {
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + segSum);
            }
        }
    }

    // 断环起点不同答案不同，枚举 n 个起点取最优（环上长度为 n 的区间）
    long long ans = 1e18;
    for (int i = 1; i <= n; i++) {
        ans = min(ans, dp[i][i + n - 1]);
    }
    cout << ans << "\n";
    return 0;
}
