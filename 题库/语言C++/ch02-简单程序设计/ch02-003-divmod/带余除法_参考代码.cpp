#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int a, b;
    cin >> a >> b;
    cout << a / b << " " << a % b << "\n";  // 整除与取余：C++ 默认向零取整
    return 0;
}
