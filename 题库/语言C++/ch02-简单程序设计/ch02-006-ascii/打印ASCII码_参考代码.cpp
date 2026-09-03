#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    char ch;
    cin >> ch;                    // cin 读 char 会自动跳过空白字符
    cout << (int)ch << "\n";      // 强制类型转换：char -> int，得到 ASCII 码
    return 0;
}
