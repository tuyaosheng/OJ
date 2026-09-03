#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <iomanip>  —— fixed、setprecision

int main()
{
    double w, cost;
    cin >> w;
    if (w <= 20)             // 边界 20 属于低费率段
        cost = w * 1.68;
    else
        cost = w * 1.98;
    // fixed + setprecision(2)：固定小数格式，保留 2 位
    cout << fixed << setprecision(2) << cost << "\n";
    return 0;
}
