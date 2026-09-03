#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int a, b;
    char op;                  // 运算符是一个字符
    cin >> a >> op >> b;      // cin 会自动跳过空格，依次读 数-符-数
    switch (op)               // 按运算符分情况处理
    {
    case '+': cout << a + b << "\n"; break;
    case '-': cout << a - b << "\n"; break;
    case '*': cout << a * b << "\n"; break;
    case '/':
        if (b == 0)                            // 先判除零，再做除法
            cout << "Divided by zero!" << "\n";
        else
            cout << a / b << "\n";             // 整数除法，舍去小数
        break;
    default:                                   // 其余字符都是无效运算符
        cout << "Invalid operator!" << "\n";
    }
    return 0;
}
