#include <cstdio>
#include <algorithm>

const int MAXW = 1005;
int dp[MAXW];
int W, n;

int main() {
    scanf("%d %d", &W, &n);
    for (int i = 1; i <= n; i++) {
        int w, p;
        scanf("%d %d", &w, &p);
        for (int j = W; j >= w; j--) {
            dp[j] = std::max(dp[j], dp[j - w] + p);
        }
    }
    printf("%d\n", dp[W]);
    return 0;
}
