#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    // 逐行输出图形：开头空格数依次是 2、1、0、1、2
    cout << "  *" << "\n";      // 2 个空格 + 1 个星
    cout << " ***" << "\n";     // 1 个空格 + 3 个星
    cout << "*****" << "\n";    // 5 个星
    cout << " ***" << "\n";     // 与第 2 行对称
    cout << "  *" << "\n";      // 与第 1 行对称
    return 0;
}
