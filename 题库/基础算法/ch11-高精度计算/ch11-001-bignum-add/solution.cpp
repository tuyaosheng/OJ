// 高精度加法：用数组模拟小学列竖式，逐位相加、逢十进一。
// 思路：字符串读入 -> 倒序存进 int 数组（下标 0 = 个位）-> 逐位加带进位 -> 倒序输出。
// 复杂度：O(n)，n 为较长数的位数（本题 ≤ 500）。
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

const int N = 600;      // 500 位，再留点余量（可能进位多出 1 位）

int a[N], b[N], c[N];   // 倒着存：a[0] 是个位，a[1] 是十位……

int main()
{
    string sa, sb;
    cin >> sa >> sb;    // 大数必须用字符串读，int/long long 都会溢出

    int la = sa.size(), lb = sb.size();

    // 倒序存入数组：字符串最后一个字符（个位）放到数组下标 0
    // 这样低位在前，进位时往下标增大的方向走，最自然
    for (int i = 0; i < la; i++)
        a[i] = sa[la - 1 - i] - '0';    // 字符转数字
    for (int i = 0; i < lb; i++)
        b[i] = sb[lb - 1 - i] - '0';

    int len = max(la, lb);              // 结果至少这么长
    int carry = 0;                      // 进位，初始为 0

    for (int i = 0; i < len; i++)
    {
        // 短的那个数，高位自动是 0（全局数组默认值），正好当 0 参与运算
        int sum = a[i] + b[i] + carry;
        c[i] = sum % 10;                // 本位只留个位数字
        carry = sum / 10;               // 满十进一
    }

    // 最高位还有进位，要再补一位。例如 999 + 1 = 1000，位数变长了
    if (carry > 0)
    {
        c[len] = carry;
        len++;
    }

    // 数组是倒着存的，所以从高位（下标大）往低位（下标 0）输出
    for (int i = len - 1; i >= 0; i--)
        cout << c[i];
    cout << "\n";

    return 0;
}
