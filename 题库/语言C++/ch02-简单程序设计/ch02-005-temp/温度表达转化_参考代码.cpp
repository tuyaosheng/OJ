#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <iomanip>  —— fixed、setprecision

int main()
{
    double f;
    cin >> f;
    // 注意：5 和 9 参与的是浮点运算（f 是 double），不会发生整数除法截断
    double c = 5 * (f - 32) / 9;
    cout << fixed << setprecision(5) << c << "\n";     // 保留 5 位小数
    return 0;
}
