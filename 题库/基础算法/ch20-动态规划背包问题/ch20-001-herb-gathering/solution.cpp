#include <cstdio>
#include <algorithm>

const int MAXT = 1005;
int dp[MAXT];
int T, n;

int main() {
    scanf("%d %d", &T, &n);
    for (int i = 1; i <= n; i++) {
        int t, v;
        scanf("%d %d", &t, &v);
        for (int j = T; j >= t; j--) {           // 一维滚动数组：j 必须从大到小
            dp[j] = std::max(dp[j], dp[j - t] + v);
        }
    }
    printf("%d\n", dp[T]);
    return 0;
}
