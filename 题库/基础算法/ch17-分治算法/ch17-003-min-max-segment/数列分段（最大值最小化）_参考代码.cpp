#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
typedef long long ll;

const int MAXN = 100005;
ll a[MAXN];
int n, m;

// 判定函数：能否把序列分成不超过 m 段，使每段和都 <= limit
bool check(ll limit) {
    int segments = 1;
    ll cur = 0;
    for (int i = 1; i <= n; i++) {
        if (cur + a[i] > limit) {   // 再加就超过限制了，新开一段
            segments++;
            cur = a[i];
        } else {
            cur += a[i];
        }
    }
    return segments <= m;
}

int main() {
    cin >> n >> m;
    ll maxA = 0, sum = 0;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        if (a[i] > maxA) maxA = a[i];
        sum += a[i];
    }

    // 二分答案：分治的“分”体现在把 [L, R] 不断对半切开逼近最优解
    ll L = maxA, R = sum;
    while (L < R) {
        ll mid = L + (R - L) / 2;
        if (check(mid)) {
            R = mid;       // mid 可行，答案可能更小，去左半区间找
        } else {
            L = mid + 1;   // mid 不可行，答案一定更大，去右半区间找
        }
    }
    cout << L << "\n";
    return 0;
}
