#include <cstdio>
#include <algorithm>

const int MAXN = 5005;
long long a[MAXN];
int dp[MAXN];
int n;

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%lld", &a[i]);

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        dp[i] = 1;   // 边界：只选它自己
        for (int j = 1; j < i; j++) {
            if (a[j] < a[i]) dp[i] = std::max(dp[i], dp[j] + 1);
        }
        ans = std::max(ans, dp[i]);
    }

    printf("%d\n", ans);
    return 0;
}
