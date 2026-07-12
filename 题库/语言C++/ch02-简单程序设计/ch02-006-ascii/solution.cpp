#include <iostream>
using namespace std;

int main()
{
    char ch;
    cin >> ch;                    // cin 读 char 会自动跳过空白字符
    cout << (int)ch << endl;      // 强制类型转换：char -> int，得到 ASCII 码
    return 0;
}
