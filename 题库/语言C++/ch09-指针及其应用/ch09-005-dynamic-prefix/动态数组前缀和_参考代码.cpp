#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int n;
    cin >> n;
    long long *b = new long long[n + 1];   // 动态数组，按需申请
    b[0] = 0;
    for (int i = 1; i <= n; i++)
    {
        long long x;
        cin >> x;
        b[i] = b[i - 1] + x;         // 前缀和递推
    }
    for (int i = 1; i <= n; i++)
    {
        if (i > 1) cout << " ";
        cout << b[i];
    }
    cout << "\n";
    delete[] b;                      // 释放动态内存，好习惯
    return 0;
}
