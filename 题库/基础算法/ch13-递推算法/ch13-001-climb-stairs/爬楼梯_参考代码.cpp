#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//
// 爬楼梯（每次 1/2/3 级）：递推的入门题。
//
// 推导：盯住【最后一步】——要到第 i 级，最后一步只可能是
//   从 i-1 迈 1 级、从 i-2 迈 2 级、从 i-3 迈 3 级，三种情况不重不漏。
//   => f[i] = f[i-1] + f[i-2] + f[i-3]   （Tribonacci 数列）
// 边界：f[0] = 1（不动也算一种）、f[1] = 1、f[2] = 2。
//
// 复杂度：O(n)，空间用滚动变量优化到 O(1)。

const long long MOD = 1000000007;

int main()
{
    int n;
    cin >> n;

    // 滚动变量：只需记住最近三项，不必开 100 万的数组
    long long f0 = 1;   // f[0] = 1，站着不动也算一种走法（关键边界！）
    long long f1 = 1;   // f[1] = 1
    long long f2 = 2;   // f[2] = 2（1+1 或 2）

    if (n == 1) { cout << 1 << "\n"; return 0; }
    if (n == 2) { cout << 2 << "\n"; return 0; }

    long long cur = 0;

    for (int i = 3; i <= n; i++)
    {
        // 三项相加最大约 3 * 10^9，超过 int 上限，所以用 long long
        // 每一步都取模，防止数值爆炸（取模不影响最终结果）
        cur = (f0 + f1 + f2) % MOD;

        // 滚动：整体往前挪一位，为下一轮做准备
        f0 = f1;
        f1 = f2;
        f2 = cur;
    }

    cout << cur << "\n";

    return 0;
}
