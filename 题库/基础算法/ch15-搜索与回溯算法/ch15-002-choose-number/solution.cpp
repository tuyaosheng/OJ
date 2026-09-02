// 选数：从 n 个数中任选 k 个相加，统计"和为素数"的选法数。
//
// = 组合枚举（上一题）+ 素数判定
//
// 组合枚举：用 start 参数强制递增下标 => 每个数只选一次、不重不漏。
// 把 sum 作为参数一路传下去，走到底直接判断，无需回头累加、也无需撤销。
//
// 素数判定：只需试除到 sqrt(x) —— 因为若 x 有大于 √x 的因子 a，
//   必有对应的因子 x/a 小于 √x，所以小范围内没找到，大范围内也不会有。
#include <iostream>
using namespace std;

const int N = 25;

int x[N];
int n, k;
int ans = 0;

// 判断 x 是否为素数
bool isPrime(int v)
{
    if (v < 2) return false;        // ★ 0 和 1 都不是素数！最常见的错误

    // 只需试除到 sqrt(v)，把 O(v) 降到 O(√v)
    // 用 (long long)i * i 防止 i*i 溢出
    for (int i = 2; (long long)i * i <= v; i++)
        if (v % i == 0)
            return false;           // 找到因子，不是素数

    return true;
}

// pos   = 已经选了几个数
// start = 这一层最小从哪个【下标】开始选（保证递增 => 不重不漏）
// sum   = 当前已选数字之和（作为参数携带，无需撤销）
void dfs(int pos, int start, int sum)
{
    if (pos == k)                   // 已经选够 k 个数了
    {
        if (isPrime(sum))           // 判断这 k 个数的和是不是素数
            ans++;
        return;
    }

    for (int i = start; i < n; i++)
    {
        // 选下标 i 这个数；下一层从 i+1 开始（严格递增，每个数只选一次）
        dfs(pos + 1, i + 1, sum + x[i]);
    }
}

int main()
{
    cin >> n >> k;

    for (int i = 0; i < n; i++)
        cin >> x[i];

    dfs(0, 0, 0);       // 已选 0 个，从下标 0 开始，当前和为 0

    cout << ans << "\n";

    return 0;
}
