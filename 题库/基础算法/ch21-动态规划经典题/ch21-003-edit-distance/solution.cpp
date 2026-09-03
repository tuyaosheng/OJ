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

    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (A[i] == B[j]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + std::min({dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]});
            }
        }
    }

    printf("%d\n", dp[n][m]);
    return 0;
}
