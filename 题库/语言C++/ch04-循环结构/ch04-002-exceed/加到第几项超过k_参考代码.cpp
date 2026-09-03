#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    long long k, s = 0;
    long long n = 0;         // 当前加到第几项
    cin >> k;
    while (s <= k)           // 还没超过 k 就继续循环
    {
        n++;                 // 项数加一
        s += n;              // 加上第 n 项
    }
    cout << n << " " << s << "\n";
    return 0;
}
