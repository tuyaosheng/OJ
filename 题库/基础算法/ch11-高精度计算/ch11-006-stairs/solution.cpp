// 数楼梯：每次走 1 级或 2 级，问走到第 n 级有多少种方案。
// 递推：到第 i 级，最后一步要么从 i-1 迈 1 级、要么从 i-2 迈 2 级
//       => f[i] = f[i-1] + f[i-2]，即斐波那契。边界 f[0] = 1, f[1] = 1。
// 难点：n ≤ 5000 时 f[5000] 有 1046 位，long long 远远不够，必须高精度。
// 做法：用滚动数组只保留最近两项，每项是一个倒序存放的高精度数。
// 复杂度：O(n * 位数) ≈ 5000 * 1050，约 500 万次基本运算，瞬间完成。
#include <iostream>
using namespace std;

const int L = 1100;     // f[5000] 约 1046 位，开 1100 够用

// 三个高精度数（倒序存放，下标 0 是个位）：
// prev2 = f[i-2], prev1 = f[i-1], cur = f[i]
int prev2[L], prev1[L], cur[L];
int len2 = 1, len1 = 1, lenC = 1;   // 各自的位数

int main()
{
    int n;
    cin >> n;

    // 边界：f[0] = 1（站着不动也算一种走法），f[1] = 1（只能迈 1 级）
    prev2[0] = 1;   len2 = 1;       // f[0]
    prev1[0] = 1;   len1 = 1;       // f[1]

    // n = 0 或 n = 1 时，答案直接就是 1
    if (n <= 1)
    {
        cout << 1 << "\n";
        return 0;
    }

    for (int i = 2; i <= n; i++)
    {
        // 高精度加法：cur = prev1 + prev2
        int len = max(len1, len2);
        int carry = 0;

        for (int j = 0; j < len; j++)
        {
            int s = prev1[j] + prev2[j] + carry;    // 短的一方高位是 0
            cur[j] = s % 10;
            carry = s / 10;
        }

        if (carry > 0)      // 最高位有进位，位数增加
        {
            cur[len] = carry;
            len++;
        }

        lenC = len;

        // 滚动：把 prev1 挪到 prev2，把 cur 挪到 prev1，为下一轮做准备
        for (int j = 0; j < len1; j++)
            prev2[j] = prev1[j];
        len2 = len1;

        for (int j = 0; j < lenC; j++)
            prev1[j] = cur[j];
        len1 = lenC;
    }

    // 此时 prev1 就是 f[n]，倒着输出（从高位到低位）
    for (int i = len1 - 1; i >= 0; i--)
        cout << prev1[i];
    cout << "\n";

    return 0;
}
