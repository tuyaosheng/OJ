#include <cstdio>

typedef long long ll;
const ll MOD = 1000000007;

const int MAXS = 1005;
ll dp[MAXS];
int n, S;

int main() {
    scanf("%d %d", &n, &S);
    dp[0] = 1;
    for (int i = 1; i <= n; i++) {
        int w;
        scanf("%d", &w);
        for (int j = w; j <= S; j++) {
            dp[j] = (dp[j] + dp[j - w]) % MOD;
        }
    }
    printf("%lld\n", dp[S]);
    return 0;
}
