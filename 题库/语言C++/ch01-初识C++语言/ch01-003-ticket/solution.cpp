#include <iostream>
using namespace std;

int main()
{
    int x, y;            // x：小朋友人数；y：总票价
    cin >> x;            // ① 输入人数
    y = 10 * x;          // ② 每张票 10 元，总价 = 10 × 人数
    cout << y << endl;   // ③ 输出总票价
    return 0;
}
