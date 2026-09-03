#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int n;
    cin >> n;
    long long sum1 = 0, sum2 = 0;    // 和可达 5*10^11，必须用 long long
    for (int i = 1; i <= n; i++)     // i 依次取 1,2,...,n
    {
        if (i % 2 == 1)              // 奇数
            sum1 += i;               // 累加：sum1 = sum1 + i
        else                         // 偶数
            sum2 += i;
    }
    cout << sum1 << " " << sum2 << "\n";
    return 0;
}
