#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout（也用来读写 string）
//   <string>   —— string
//   <algorithm> —— min

const int MAXN = 1005;
int dp[MAXN][MAXN];   // dp[i][j] = A 的前 i 个字符变成 B 的前 j 个字符最少需要几步

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string A, B;
    cin >> A >> B;
    int n = A.size(), m = B.size();

    for (int i = 0; i <= n; i++) dp[i][0] = i;   // B 是空串：删光 A 的前 i 个字符
    for (int j = 0; j <= m; j++) dp[0][j] = j;   // A 是空串：插出 B 的前 j 个字符

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];   // 末位已经相同，不用操作
            } else {
                // 三选一取最优：dp[i-1][j-1]=替换，dp[i-1][j]=删除 A 末位，dp[i][j-1]=给 A 插入一位
                dp[i][j] = 1 + min({dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]});
            }
        }
    }

    cout << dp[n][m] << "\n";
    return 0;
}
