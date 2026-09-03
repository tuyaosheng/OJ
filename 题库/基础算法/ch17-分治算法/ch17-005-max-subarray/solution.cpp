#include <cstdio>
#include <algorithm>

typedef long long ll;

const int MAXN = 100005;
ll a[MAXN];
int n;

// “合”步骤：算出横跨中点 mid 的最大子段和，O(n)
ll crossMax(int l, int mid, int r) {
    ll leftMax = -4e18, sum = 0;
    for (int i = mid; i >= l; i--) {
        sum += a[i];
        leftMax = std::max(leftMax, sum);
    }
    ll rightMax = -4e18;
    sum = 0;
    for (int i = mid + 1; i <= r; i++) {
        sum += a[i];
        rightMax = std::max(rightMax, sum);
    }
    return leftMax + rightMax;
}

// 分治求 [l, r] 区间的最大子段和
ll maxSubarray(int l, int r) {
    if (l == r) return a[l];            // 递归终止条件：只剩 1 个数
    int mid = (l + r) / 2;
    ll leftBest = maxSubarray(l, mid);       // 子问题：完全在左半
    ll rightBest = maxSubarray(mid + 1, r);  // 子问题：完全在右半
    ll cross = crossMax(l, mid, r);          // 合并：横跨中点
    return std::max({leftBest, rightBest, cross});
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%lld", &a[i]);
    printf("%lld\n", maxSubarray(1, n));
    return 0;
}
