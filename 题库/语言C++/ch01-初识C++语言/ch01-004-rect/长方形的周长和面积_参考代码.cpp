#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int a, b;                    // 长方形的长和宽
    cin >> a >> b;               // 一次读入两个整数（cin 会自动跳过空格）
    int c = (a + b) * 2;         // 周长公式，注意括号：先加后乘
    int s = a * b;               // 面积公式
    cout << c << " " << s << "\n";  // 输出周长和面积，中间隔一个空格
    return 0;
}
