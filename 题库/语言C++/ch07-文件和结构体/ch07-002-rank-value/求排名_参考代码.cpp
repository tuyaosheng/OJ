#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— sort

struct Node {
    int val;    // 数值
    int pos;    // 在输入中的原始位置（下标）
};

bool cmp(const Node &a, const Node &b)
{
    return a.val < b.val;    // 按数值从小到大排
}

int main()
{
    int n;
    cin >> n;
    Node a[10005];
    int rankArr[10005];      // rankArr[pos] = 该位置的数的排名
    for (int i = 0; i < n; i++)
    {
        cin >> a[i].val;
        a[i].pos = i;
    }
    sort(a, a + n, cmp);     // 数互不相同，普通 sort 即可
    for (int i = 0; i < n; i++)
        rankArr[a[i].pos] = i + 1;   // 排好序后第 i 个（从 0）的排名是 i+1
    for (int i = 0; i < n; i++)
    {
        if (i > 0) cout << " ";
        cout << rankArr[i];
    }
    cout << "\n";
    return 0;
}
