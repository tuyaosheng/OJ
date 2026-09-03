#include <cstdio>
#include <algorithm>

const int MAXN = 105;
int h[MAXN], L[MAXN], R[MAXN];
int n;

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &h[i]);

    for (int i = 1; i <= n; i++) {           // 正向：以 i 结尾的最长上升子序列
        L[i] = 1;
        for (int j = 1; j < i; j++) {
            if (h[j] < h[i]) L[i] = std::max(L[i], L[j] + 1);
        }
    }
    for (int i = n; i >= 1; i--) {           // 反向：以 i 开头往右的最长下降子序列
        R[i] = 1;
        for (int j = n; j > i; j--) {
            if (h[j] < h[i]) R[i] = std::max(R[i], R[j] + 1);
        }
    }

    int maxLen = 0;
    for (int i = 1; i <= n; i++) {
        maxLen = std::max(maxLen, L[i] + R[i] - 1);
    }

    printf("%d\n", n - maxLen);
    return 0;
}
