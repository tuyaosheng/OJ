#include <cstdio>
#include <algorithm>

typedef long long ll;

const int MAXT = 100005;
ll dp[MAXT];
int T, n;

int main() {
    scanf("%d %d", &T, &n);
    for (int i = 1; i <= n; i++) {
        int t, v;
        scanf("%d %d", &t, &v);
        for (int j = t; j <= T; j++) {           // 完全背包：j 从小到大
            dp[j] = std::max(dp[j], dp[j - t] + v);
        }
    }
    printf("%lld\n", dp[T]);
    return 0;
}
