#include <cstdio>
#include <algorithm>

const int MAXV = 20005;
int dp[MAXV];
int V, n;

int main() {
    scanf("%d", &V);
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int v;
        scanf("%d", &v);
        for (int j = V; j >= v; j--) {
            dp[j] = std::max(dp[j], dp[j - v] + v);
        }
    }
    printf("%d\n", V - dp[V]);
    return 0;
}
