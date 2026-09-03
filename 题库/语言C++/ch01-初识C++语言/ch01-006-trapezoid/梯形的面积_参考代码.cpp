#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <iomanip>  —— fixed、setprecision

int main()
{
    double a, b, s;          // 用 double 存储，避免整数除法丢失小数
    cin >> a >> b >> s;
    double h = 2 * s / a;             // 由三角形面积 s = a*h/2 反推高 h
    double ans = (a + b) * h / 2;     // 梯形面积公式：(上底+下底)*高/2
    cout << fixed << setprecision(2) << ans << "\n";     // 保留两位小数输出
    return 0;
}
