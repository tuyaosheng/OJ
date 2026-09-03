#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int a[100005];

int main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    long long sum = 0;
    long long maxv = a[0];
    // p 从数组首地址出发，逐个后移遍历
    for (int *p = a; p < a + n; p++)
    {
        sum += *p;                   // *p 就是当前元素
        if (*p > maxv) maxv = *p;
    }
    cout << sum << " " << maxv << "\n";
    return 0;
}
