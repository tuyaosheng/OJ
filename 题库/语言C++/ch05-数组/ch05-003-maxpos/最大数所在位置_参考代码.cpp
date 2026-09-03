#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int a[10005];

int main()
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];
    int maxa = a[1], k = 1;          // 擂台初值：第 1 个数和它的位置
    for (int i = 2; i <= n; i++)
    {
        if (a[i] > maxa)             // 严格大于：相等不更新，保住首个位置
        {
            maxa = a[i];
            k = i;                   // 值和位置一起更新
        }
    }
    cout << k << "\n";
    return 0;
}
