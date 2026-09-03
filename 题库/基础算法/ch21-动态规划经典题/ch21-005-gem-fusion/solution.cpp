#include <cstdio>
#include <cstring>
#include <algorithm>

const int MAXN = 205;
int a[MAXN];
long long prefix[MAXN];
long long dp[MAXN][MAXN];
int n;

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        a[i + n] = a[i];
    }
    int m = 2 * n;
    for (int i = 1; i <= m; i++) prefix[i] = prefix[i - 1] + a[i];

    memset(dp, 0, sizeof(dp));
    for (int len = 2; len <= n; len++) {
        for (int i = 1; i + len - 1 <= m; i++) {
            int j = i + len - 1;
            dp[i][j] = 1e18;
            long long segSum = prefix[j] - prefix[i - 1];
            for (int k = i; k < j; k++) {
                dp[i][j] = std::min(dp[i][j], dp[i][k] + dp[k + 1][j] + segSum);
            }
        }
    }

    long long ans = 1e18;
    for (int i = 1; i <= n; i++) {
        ans = std::min(ans, dp[i][i + n - 1]);
    }
    printf("%lld\n", ans);
    return 0;
}
