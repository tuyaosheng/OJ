#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int a;
    cin >> a;
    if (a % 2 == 0)          // 能被 2 整除即偶数（0 和负偶数也成立）
        cout << "yes" << "\n";
    else                     // 否则是奇数（负奇数 a%2 为 -1，也会走这里）
        cout << "no" << "\n";
    return 0;
}
