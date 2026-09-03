#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int t;
    cin >> t;
    if (t >= 25 && t <= 30)      // 区间判断：两个条件用 && (并且) 连接
        cout << "ok" << "\n";
    else
        cout << "no" << "\n";
    return 0;
}
