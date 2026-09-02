// 我的世界·合成计算
// 考点：乘法、整除、取余
// 1 木头 = 4 木板；6 木板 = 1 门
#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;
    int planks = 4 * n;              // 全部木头先合成木板
    cout << planks / 6 << " "        // 整除：能做几扇门
         << planks % 6 << endl;      // 取余：剩几块木板
    return 0;
}
