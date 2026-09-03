#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— sort
typedef long long ll;

const int MAXN = 100005;
ll pos[MAXN];
int n, k;

// 判定函数：以最小间距 mid 贪心选点，能否选出至少 k 个
bool check(ll mid) {
    int count = 1;      // 第一个点必选
    ll last = pos[1];
    for (int i = 2; i <= n; i++) {
        if (pos[i] - last >= mid) {
            count++;
            last = pos[i];
        }
    }
    return count >= k;
}

int main() {
    cin >> n >> k;
    for (int i = 1; i <= n; i++) cin >> pos[i];
    sort(pos + 1, pos + n + 1);

    ll L = 0, R = pos[n] - pos[1];
    while (L < R) {
        ll mid = L + (R - L + 1) / 2;   // 向上取整，避免死循环
        if (check(mid)) {
            L = mid;        // mid 可行，答案可能更大，去右半区间找
        } else {
            R = mid - 1;    // mid 不可行，答案一定更小，去左半区间找
        }
    }
    cout << L << "\n";
    return 0;
}
