#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout（也用来读入 string）
//   <string>   —— string
//   <algorithm> —— max

const int MAXN = 1005;
int dp[MAXN][MAXN];

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
                dp[i][j] = dp[i - 1][j - 1] + 1;
                ans = max(ans, dp[i][j]);
            } else {
                dp[i][j] = 0;
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
