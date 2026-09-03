#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int n, m;                // n：苹果总数，m：每袋的个数
    cin >> n >> m;
    int bags = n / m;        // 整数除法：自动舍去小数，得到装满的袋子数
    int rest = n % m;        // 取余运算：得到装不满的剩余苹果数
    cout << bags << " " << rest << "\n";
    return 0;
}
