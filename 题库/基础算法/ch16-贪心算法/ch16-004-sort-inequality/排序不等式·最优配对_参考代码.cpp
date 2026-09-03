#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>   —— cin / cout
//   <algorithm>  —— sort
//   <functional> —— greater
// 考点：排序不等式（乱序和 <= 乱序和 <= 顺序和 的最小情形）
// 结论：一个升序、一个降序配对，乘积和最小

int a[1005], b[1005];

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) cin >> b[i];

    sort(a, a + n);                      // a 升序
    sort(b, b + n, greater<int>());      // b 降序：大的配小的

    long long ans = 0;
    for (int i = 0; i < n; i++) ans += (long long)a[i] * b[i];
    cout << ans << "\n";
    return 0;
}
