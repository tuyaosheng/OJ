#include <cstdio>
#include <cstring>
#include <algorithm>

const int MAXN = 1005;
char A[MAXN], B[MAXN];
int dp[MAXN][MAXN];

int main() {
    scanf("%s", A + 1);
    scanf("%s", B + 1);
    int n = strlen(A + 1), m = strlen(B + 1);

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (A[i] == B[j]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                ans = std::max(ans, dp[i][j]);
            } else {
                dp[i][j] = 0;
            }
        }
    }

    printf("%d\n", ans);
    return 0;
}
