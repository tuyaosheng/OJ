#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

long long d[100005];

int main()
{
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    while (m--)
    {
        int l, r, v;
        cin >> l >> r >> v;
        d[l] += v;                   // 差分：区间修改 O(1)
        d[r + 1] -= v;
    }
    long long cur = 0;
    for (int i = 1; i <= n; i++)
    {
        cur += d[i];                 // 前缀和还原出真实值
        if (i > 1) cout << " ";
        cout << cur;
    }
    cout << "\n";
    return 0;
}
