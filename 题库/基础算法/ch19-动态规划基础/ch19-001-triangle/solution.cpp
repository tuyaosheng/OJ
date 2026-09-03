#include <cstdio>
#include <algorithm>

typedef long long ll;

const int MAXN = 1005;
ll a[MAXN][MAXN];
ll dp[MAXN][MAXN];
int n;

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= i; j++)
            scanf("%lld", &a[i][j]);

    for (int j = 1; j <= n; j++) dp[n][j] = a[n][j];   // 边界：最后一行

    for (int i = n - 1; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            dp[i][j] = a[i][j] + std::max(dp[i + 1][j], dp[i + 1][j + 1]);
        }
    }

    printf("%lld\n", dp[1][1]);
    return 0;
}
