// 迷你汉诺塔（只求步数）
// 考点：递归函数、递归边界、long long
// T(1) = 1, T(n) = 2*T(n-1) + 1，封闭形式是 2^n - 1
#include <iostream>
using namespace std;

long long T(int n)
{
    if (n == 1) return 1;         // 边界：1 个齿轮 1 步
    return 2 * T(n - 1) + 1;      // 先移走上面 n-1 个，移最大的，再移回来
}

int main()
{
    int n;
    cin >> n;
    cout << T(n) << endl;
    return 0;
}
