#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <iomanip>  —— fixed、setprecision

const double PI = 3.14159;   // 符号常量：见名知意，且要改只需改这一处

int main()
{
    double r, h;
    cin >> r >> h;
    // 表面积 = 上下底面积(2*PI*r*r) + 侧面积(2*PI*r*h)
    double s = 2 * PI * r * r + 2 * PI * r * h;
    cout << fixed << setprecision(3) << s << "\n";     // 保留 3 位小数
    return 0;
}
