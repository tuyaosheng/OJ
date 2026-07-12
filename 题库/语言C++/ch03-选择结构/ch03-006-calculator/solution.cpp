#include <iostream>
using namespace std;

int main()
{
    int a, b;
    char op;                  // 运算符是一个字符
    cin >> a >> op >> b;      // cin 会自动跳过空格，依次读 数-符-数
    switch (op)               // 按运算符分情况处理
    {
    case '+': cout << a + b << endl; break;
    case '-': cout << a - b << endl; break;
    case '*': cout << a * b << endl; break;
    case '/':
        if (b == 0)                            // 先判除零，再做除法
            cout << "Divided by zero!" << endl;
        else
            cout << a / b << endl;             // 整数除法，舍去小数
        break;
    default:                                   // 其余字符都是无效运算符
        cout << "Invalid operator!" << endl;
    }
    return 0;
}
