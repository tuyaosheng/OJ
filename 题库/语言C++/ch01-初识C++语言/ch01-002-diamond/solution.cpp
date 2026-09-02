#include <iostream>
using namespace std;

int main()
{
    // 逐行输出图形：开头空格数依次是 2、1、0、1、2
    cout << "  *" << endl;      // 2 个空格 + 1 个星
    cout << " ***" << endl;     // 1 个空格 + 3 个星
    cout << "*****" << endl;    // 5 个星
    cout << " ***" << endl;     // 与第 2 行对称
    cout << "  *" << endl;      // 与第 1 行对称
    return 0;
}
