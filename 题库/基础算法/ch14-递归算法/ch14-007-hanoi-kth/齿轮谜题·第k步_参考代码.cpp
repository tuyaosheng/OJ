#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
// 考点：利用汉诺塔递归结构定位第 k 步（分治递归）
// n 盘最优解 = [n-1 盘子问题] + [移第 n 号盘] + [n-1 盘子问题]

int solve(int n, long long k)
{
    long long mid = 1LL << (n - 1);        // 第 n 号盘恰好在正中间这一步移动
    if (k == mid) return n;                // 命中中点：移的就是最大盘
    if (k < mid) return solve(n - 1, k);   // 前半段：n-1 盘的子问题
    return solve(n - 1, k - mid);          // 后半段：同样的子问题，步数平移
}

int main()
{
    int n;
    long long k;                            // k 最大 2^60-1，必须 long long
    cin >> n >> k;
    cout << solve(n, k) << "\n";
    return 0;
}
