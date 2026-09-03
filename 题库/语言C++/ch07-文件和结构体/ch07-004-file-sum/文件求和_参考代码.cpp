#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    long long sum = 0;
    int x;
    while (cin >> x)     // 一直读到文件末尾（EOF）；读取失败则退出循环
        sum += x;
    cout << sum << "\n";
    return 0;
}
