#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout（也用来读写 string）
//   <string>   —— string
//   <algorithm> —— max

const int MAXN = 1005;
int dp[MAXN][MAXN];   // dp[i][j] = 以 A[i-1]、B[j-1] 结尾的公共子串长度（必须连续）

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string A, B;
    cin >> A >> B;
    int n = A.size(), m = B.size();

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;   // 接上一位，连续段变长
                ans = max(ans, dp[i][j]);
            } else {
                dp[i][j] = 0;   // 子串要求连续，一旦断开必须清零重新开始（这是和最长公共子序列的关键区别）
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
