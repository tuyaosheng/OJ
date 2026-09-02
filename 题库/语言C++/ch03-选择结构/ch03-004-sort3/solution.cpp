#include <iostream>
using namespace std;

int main()
{
    int a, b, c, t;
    cin >> a >> b >> c;
    if (a < b) { t = a; a = b; b = t; }   // 保证 a >= b
    if (a < c) { t = a; a = c; c = t; }   // 保证 a >= c，此时 a 已是最大
    if (b < c) { t = b; b = c; c = t; }   // 保证 b >= c，三数有序
    cout << a << " " << b << " " << c << endl;
    return 0;
}
