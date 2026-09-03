#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout、fixed / setprecision
//   <algorithm> —— sort
//   <iomanip>   —— setprecision
// 考点：排序贪心 + 交换论证
// 结论：打水时间短的先打，平均等待时间最短

int t[1005];

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) cin >> t[i];
    sort(t, t + n);                 // 从小到大排序：贪心的核心一步

    long long total = 0, wait = 0;  // wait: 目前为止排在前面的人共用掉多少时间
    for (int i = 0; i < n; i++) {
        total += wait;               // 当前这位同学要等这么久
        wait += t[i];                // 他打完后，后面的人又要多等这么久
    }
    cout << fixed << setprecision(2) << (double)total / n << "\n";
    return 0;
}
