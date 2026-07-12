// 高精度乘高精度：竖式错位相加。
// 核心：倒序存数后，a 的第 i 位 乘 b 的第 j 位，结果加到答案的第 (i + j) 位上。
// 策略：先只管把所有乘积累加到对应位（不管进位），最后再统一扫一遍处理进位。
// 复杂度：O(la * lb)，本题 500 * 500 = 25 万次，很快。
#include <iostream>
#include <string>
using namespace std;

const int N = 1100;     // 结果最多 la + lb = 1000 位，留余量

int a[N], b[N], c[N];   // c 开两倍长，存乘积

int main()
{
    string sa, sb;
    cin >> sa >> sb;

    int la = sa.size(), lb = sb.size();

    // 倒序存入：下标 0 是个位。这样下标就等于"这一位代表 10 的几次方"
    for (int i = 0; i < la; i++)
        a[i] = sa[la - 1 - i] - '0';
    for (int i = 0; i < lb; i++)
        b[i] = sb[lb - 1 - i] - '0';

    // 第一步：错位相加。a[i] * 10^i 乘 b[j] * 10^j = a[i]*b[j] * 10^(i+j)
    // 所以乘积落在第 i + j 位。这里先不管进位，让 c[i+j] 累加成一个大数
    for (int i = 0; i < la; i++)
        for (int j = 0; j < lb; j++)
            c[i + j] += a[i] * b[j];
    // 每一位最多累加 500 次 9*9=81，即 40500，int 完全够用

    int len = la + lb;      // 结果最长不超过 la + lb 位

    // 第二步：统一处理进位。此刻 c[i] 可能是个多位数，把超出个位的部分推给高位
    for (int i = 0; i < len - 1; i++)
    {
        c[i + 1] += c[i] / 10;      // 进位加到高一位
        c[i] %= 10;                 // 本位只留个位数字
    }

    // 第三步：去前导零，至少保留一位（否则 0 * 12345 会输出一长串 0）
    while (len > 1 && c[len - 1] == 0)
        len--;

    // 倒着输出：从高位到低位
    for (int i = len - 1; i >= 0; i--)
        cout << c[i];
    cout << "\n";

    return 0;
}
