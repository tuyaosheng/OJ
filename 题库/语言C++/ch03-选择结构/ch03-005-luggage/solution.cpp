#include <iostream>
#include <iomanip>           // fixed / setprecision 需要这个头文件
using namespace std;

int main()
{
    double w, cost;
    cin >> w;
    if (w <= 20)             // 边界 20 属于低费率段
        cost = w * 1.68;
    else
        cost = w * 1.98;
    // fixed + setprecision(2)：固定小数格式，保留 2 位
    cout << fixed << setprecision(2) << cost << endl;
    return 0;
}
