#include <iostream>
using namespace std;

int main()
{
    int a, b, c;
    cin >> a >> b >> c;
    int ans = (a + b) * c;   // 括号保证先算加法再算乘法
    cout << ans << endl;
    return 0;
}
