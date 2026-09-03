#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int a, b, c;
    cin >> a >> b >> c;
    int ans = (a + b) * c;   // 括号保证先算加法再算乘法
    cout << ans << "\n";
    return 0;
}
