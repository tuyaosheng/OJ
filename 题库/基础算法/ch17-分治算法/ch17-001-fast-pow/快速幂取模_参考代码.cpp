#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
typedef long long ll;

// 分治求 a^b mod p：把指数 b 不断减半，规模减半即分治的“分”，
// 子问题算完后平方再视奇偶乘一次 a，就是“合”。
ll fastPow(ll a, ll b, ll p) {
    if (b == 0) return 1 % p;           // 递归终止条件
    ll half = fastPow(a, b / 2, p);     // 子问题：规模减半
    ll result = half * half % p;        // 合并：平方
    if (b % 2 == 1) result = result * (a % p) % p;  // b 为奇数再补乘一次 a
    return result;
}

int main() {
    ll a, b, p;
    cin >> a >> b >> p;
    a %= p;
    cout << fastPow(a, b, p) << "\n";
    return 0;
}
