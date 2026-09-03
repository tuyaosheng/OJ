#include <cstdio>
#include <algorithm>

typedef long long ll;
const ll NEG_INF = -4e18;

const int MAXN = 1005;
ll a[MAXN][MAXN];
ll dp[MAXN][MAXN];
int n, m;

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            scanf("%lld", &a[i][j]);

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= m; j++)
            dp[i][j] = NEG_INF;   // 先全部标记成“走不到”

    dp[1][1] = a[1][1];   // 边界
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (i == 1 && j == 1) continue;
            ll best = std::max(dp[i - 1][j], dp[i][j - 1]);
            dp[i][j] = a[i][j] + best;
        }
    }

    printf("%lld\n", dp[n][m]);
    return 0;
}
