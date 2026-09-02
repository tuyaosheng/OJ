// 错位排列（装错信封问题）：n 封信全部装错，求方案数 D(n)。
//
// 推导：考虑第 n 封信进了哪个信封，设它进了 k 号（k 有 n-1 种选择，不能是 n）。
//   再看第 k 封信去哪了，恰好分两种情况：
//   ① 第 k 封信进了 n 号信封（两者交换）
//      -> 剩下 n-2 封信全装错 -> D(n-2)
//   ② 第 k 封信【没进】n 号信封
//      -> 此时"第 k 封信不能进 n 号"这个限制，形式上等同于"不能进自己的信封"，
//         把 n 号信封看作第 k 封信"自己的"信封，问题化为 n-1 封信全装错 -> D(n-1)
//   => D(n) = (n-1) * [ D(n-1) + D(n-2) ]
// 边界：D(1) = 0（唯一的信只能装对）、D(2) = 1（互换）。
//
// 复杂度：O(n)。
#include <iostream>
using namespace std;

const long long MOD = 1000000007;

int main()
{
    int n;
    cin >> n;

    // 边界特判
    if (n == 1) { cout << 0 << "\n"; return 0; }
    if (n == 2) { cout << 1 << "\n"; return 0; }

    // 滚动变量，不必开 100 万的数组
    long long d1 = 0;   // D(1) = 0
    long long d2 = 1;   // D(2) = 1
    long long cur = 0;

    for (int i = 3; i <= n; i++)
    {
        // (i-1) 最大 1e6，(d2 + d1) 最大约 2e9，相乘约 2e15 —— 远超 int！
        // 必须用 long long，且乘法前先转换
        cur = (long long)(i - 1) * ((d2 + d1) % MOD) % MOD;

        // 滚动
        d1 = d2;
        d2 = cur;
    }

    cout << cur << "\n";

    return 0;
}
