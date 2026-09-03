#include <cstdio>
#include <algorithm>

const int MAXT = 1005;
int dp[MAXT];
int T, n;

int main() {
    scanf("%d %d", &T, &n);
    for (int i = 1; i <= n; i++) {
        int t, v, c;
        scanf("%d %d %d", &t, &v, &c);
        for (int j = T; j >= t; j--) {
            int maxK = std::min(c, j / t);
            for (int k = 1; k <= maxK; k++) {
                dp[j] = std::max(dp[j], dp[j - k * t] + k * v);
            }
        }
    }
    printf("%d\n", dp[T]);
    return 0;
}
