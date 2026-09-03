#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

// 判断 n 是否为素数：是返回 true，否返回 false
bool isPrime(int n)
{
    if (n < 2) return false;            // 1 及以下都不是素数
    for (long long i = 2; i * i <= n; i++)  // i 用 long long 防 i*i 溢出
        if (n % i == 0) return false;   // 找到约数，不是素数
    return true;                        // 没找到约数，是素数
}

int main()
{
    int n;
    cin >> n;
    if (isPrime(n)) cout << "yes" << "\n";
    else            cout << "no" << "\n";
    return 0;
}
