#include <cstdio>

typedef long long ll;

const int MAXN = 100005;
ll a[MAXN];
int n, m;

// 判定函数：人均页数不超过 limit 时，最少需要几个抄写员
bool check(ll limit) {
    int copiers = 1;
    ll cur = 0;
    for (int i = 1; i <= n; i++) {
        if (cur + a[i] > limit) {
            copiers++;
            cur = a[i];
        } else {
            cur += a[i];
        }
    }
    return copiers <= m;
}

int main() {
    scanf("%d %d", &n, &m);
    ll maxA = 0, sum = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &a[i]);
        if (a[i] > maxA) maxA = a[i];
        sum += a[i];
    }

    ll L = maxA, R = sum;
    while (L < R) {
        ll mid = L + (R - L) / 2;
        if (check(mid)) {
            R = mid;
        } else {
            L = mid + 1;
        }
    }
    printf("%lld\n", L);
    return 0;
}
