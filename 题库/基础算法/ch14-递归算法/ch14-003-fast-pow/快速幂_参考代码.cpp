#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//
// 快速幂：求 a^b mod p，b 可达 10^18。
//
// 朴素做法循环 b 次，O(b) = 10^18 次 —— 跑三十多年。
//
// 分治思想：a^b = (a^(b/2))^2      （b 为偶数）
//           a^b = (a^(b/2))^2 * a  （b 为奇数，整数除法丢了余数 1，要补乘一个 a）
// 每递归一层，指数【砍掉一半】=> 只需 O(log b) ≈ 60 层。

long long power(long long a, long long b, long long p)
{
    // 递归边界：a^0 = 1。
    // ★ 注意必须是 1 % p 而不是 1 —— 因为 p 可能等于 1，
    //   此时任何数 mod 1 都是 0。直接 return 1 会答案错误。
    if (b == 0) return 1 % p;

    // ★ 关键：只递归【一次】，把结果存起来再平方。
    // 千万别写成 power(a,b/2,p) * power(a,b/2,p) ——
    // 那样会重复计算，复杂度退化回 O(b)，快速幂就白写了。
    long long half = power(a, b / 2, p);

    // 平方。half 最大接近 p ≈ 10^9，平方约 10^18，
    // long long 上限 9.2e18，刚好装得下。
    long long res = half * half % p;

    // b 是奇数时，b/2 向下取整丢掉了一个 a，这里补回来
    if (b % 2 == 1)
        res = res * a % p;

    return res;
}

int main()
{
    long long a, b, p;
    cin >> a >> b >> p;     // b 可达 10^18，必须用 long long

    a %= p;                 // 先把 a 取模，防止 a 本身就比 p 大

    cout << power(a, b, p) << "\n";

    return 0;
}
