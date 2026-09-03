#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int a[100005];

bool isPrime(int n)
{
    if (n < 2) return false;
    for (long long i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

int main()
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];
    int pos = -1;
    for (int i = 1; i <= n; i++)
        if (isPrime(a[i]))           // 找到第一个质数
        {
            pos = i;
            break;                   // 立刻停止
        }
    cout << pos << "\n";
    return 0;
}
