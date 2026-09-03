#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int a[1005];

int main()
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];
    // 冒泡排序：外层控制轮数，内层两两比较相邻元素
    for (int i = 1; i < n; i++)
        for (int j = 1; j <= n - i; j++)
            if (a[j] < a[j + 1])      // 前小后大就交换 -> 大的往前冒
            {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
    for (int i = 1; i <= n; i++)
    {
        if (i > 1) cout << " ";
        cout << a[i];
    }
    cout << "\n";
    return 0;
}
