// 高精度乘单精度：大数 a 的每一位都乘以小数 b，再统一处理进位。
// 思路：倒序存 a -> 每位乘 b 加进位 -> 本位留个位、其余进位 -> 末尾把剩余进位全部展开 -> 去前导零。
// 复杂度：O(n)，n 为 a 的位数（本题 ≤ 500）。
#include <iostream>
#include <string>
using namespace std;

const int N = 600;

int a[N], c[N];

int main()
{
    string sa;
    int b;
    cin >> sa >> b;

    int la = sa.size();

    // 倒序存入数组：下标 0 是个位
    for (int i = 0; i < la; i++)
        a[i] = sa[la - 1 - i] - '0';

    int carry = 0;      // 进位（注意：这里的进位可能不止一位！）

    for (int i = 0; i < la; i++)
    {
        // a[i] 最大 9，b 最大 10000，乘积最大 90000，再加进位也远不会爆 int
        int cur = a[i] * b + carry;

        c[i] = cur % 10;    // 本位只留个位数字
        carry = cur / 10;   // 剩下的全部进位给高位
    }

    int len = la;

    // 循环结束后 carry 可能是个多位数（比如大数乘 10000），
    // 必须用 while 一位一位地全部展开，不能只补一位
    while (carry > 0)
    {
        c[len] = carry % 10;
        carry /= 10;
        len++;
    }

    // 去前导零，至少保留一位。b = 0 时结果全是 0，这一步保证输出单个 "0"
    while (len > 1 && c[len - 1] == 0)
        len--;

    // 倒着输出：从高位到低位
    for (int i = len - 1; i >= 0; i--)
        cout << c[i];
    cout << "\n";

    return 0;
}
