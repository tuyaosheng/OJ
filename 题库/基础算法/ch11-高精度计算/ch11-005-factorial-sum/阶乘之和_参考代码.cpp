#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— max
//
// 阶乘之和：S = 1! + 2! + ... + n!，n ≤ 100，100! 有 158 位，必须高精度。
// 关键：利用递推 k! = (k-1)! * k，边乘边加，不必每个阶乘都从头算。
// 用到两个高精度工具：① 高精度 × 单精度（算 fac）② 高精度 + 高精度（累加 sum）
// 复杂度：O(n * 位数)，100 * 200 级别，瞬间出结果。

const int N = 300;      // 100! 约 158 位，开 300 绰绰有余

// 高精度数一律用"倒序数组 + 长度"表示：a[0] 是个位
int fac[N], facLen = 1;     // 当前的 k!，初始为 1（0 位置放 1）
int sum[N], sumLen = 1;     // 累加和，初始为 0

// 工具① 高精度 fac 乘以小整数 k（原地修改）
void mulSmall(int a[], int &len, int k)
{
    int carry = 0;

    for (int i = 0; i < len; i++)
    {
        int cur = a[i] * k + carry;
        a[i] = cur % 10;        // 本位留个位
        carry = cur / 10;       // 其余进位
    }

    // 进位可能是多位数，要一位一位全部展开
    while (carry > 0)
    {
        a[len] = carry % 10;
        carry /= 10;
        len++;
    }
}

// 工具② 高精度 sum 加上高精度 b（原地修改 sum）
void addBig(int a[], int &lenA, int b[], int lenB)
{
    int len = max(lenA, lenB);
    int carry = 0;

    for (int i = 0; i < len; i++)
    {
        int s = a[i] + b[i] + carry;    // 短的一方高位自动是 0
        a[i] = s % 10;
        carry = s / 10;
    }

    if (carry > 0)      // 最高位还有进位，补一位
    {
        a[len] = carry;
        len++;
    }

    lenA = len;
}

int main()
{
    int n;
    cin >> n;

    fac[0] = 1;     // fac 初始为 1（注意不能是 0，否则怎么乘都是 0）
    sum[0] = 0;     // sum 初始为 0

    for (int k = 1; k <= n; k++)
    {
        mulSmall(fac, facLen, k);           // fac = fac * k，即 (k-1)! * k = k!
        addBig(sum, sumLen, fac, facLen);   // sum = sum + k!
    }

    // 倒着输出：从高位到低位
    for (int i = sumLen - 1; i >= 0; i--)
        cout << sum[i];
    cout << "\n";

    return 0;
}
