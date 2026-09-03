#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
// 考点：递归函数、递归边界、long long
// T(1) = 1, T(n) = 2*T(n-1) + 1，封闭形式是 2^n - 1

long long T(int n)
{
    if (n == 1) return 1;         // 边界：1 个齿轮 1 步
    return 2 * T(n - 1) + 1;      // 先移走上面 n-1 个，移最大的，再移回来
}

int main()
{
    int n;
    cin >> n;
    cout << T(n) << "\n";
    return 0;
}
