#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <iomanip>  —— fixed、setprecision
// 考点：double、混合运算、保留两位小数输出

int main()
{
    int atk, def;
    double mult;
    cin >> atk >> mult >> def;
    double damage = atk * mult - def;   // int 与 double 混合运算，结果是 double
    cout << fixed << setprecision(2) << damage << "\n";     // 保留两位小数
    return 0;
}
