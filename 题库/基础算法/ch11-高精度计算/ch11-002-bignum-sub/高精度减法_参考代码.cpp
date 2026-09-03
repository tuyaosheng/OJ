#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <string>    —— string
//   <algorithm> —— swap
//
// 高精度减法：数组模拟列竖式，逐位相减、不够就向高位借一。
// 思路：先比大小 -> 保证"大减小" -> 逐位减带借位 -> 去前导零 -> 该加负号就加。
// 复杂度：O(n)，n 为较长数的位数（本题 ≤ 500）。

const int N = 600;

int a[N], b[N], c[N];

// 比较两个大数的大小（sa、sb 均无前导零）
// 返回 true 表示 sa >= sb
bool greaterEqual(const string &sa, const string &sb)
{
    // 位数不同，位数多的那个更大
    if (sa.size() != sb.size())
        return sa.size() > sb.size();
    // 位数相同：从最高位往低位逐位比，第一个不同的位就能定胜负
    return sa >= sb;    // 等长的数字串，字典序比较等价于数值比较
}

int main()
{
    string sa, sb;
    cin >> sa >> sb;

    // 关键一步：如果 a < b，交换成"大减小"，并记下结果是负的
    bool negative = false;
    if (!greaterEqual(sa, sb))
    {
        swap(sa, sb);
        negative = true;    // 最后输出时补一个负号
    }

    int la = sa.size(), lb = sb.size();

    // 倒序存入数组：下标 0 是个位
    for (int i = 0; i < la; i++)
        a[i] = sa[la - 1 - i] - '0';
    for (int i = 0; i < lb; i++)
        b[i] = sb[lb - 1 - i] - '0';

    int borrow = 0;             // 借位标记：上一位是否向本位借走了 1

    for (int i = 0; i < la; i++)    // 此时 a 一定是较长（或等长）的那个
    {
        // 短的数的高位自动是 0（全局数组默认值）
        int t = a[i] - b[i] - borrow;

        if (t < 0)              // 本位不够减
        {
            t += 10;            // 向高位借 1，本位就多了 10
            borrow = 1;         // 记住高位要少 1
        }
        else
        {
            borrow = 0;
        }

        c[i] = t;
    }

    // 去掉多余的前导零，但至少保留一位（否则 5-5 会输出空）
    int len = la;
    while (len > 1 && c[len - 1] == 0)
        len--;

    // 结果是 0 的时候不能输出 "-0"，所以这里要判一下 len==1 && c[0]==0
    if (negative && !(len == 1 && c[0] == 0))
        cout << "-";

    // 倒着输出：从高位到低位
    for (int i = len - 1; i >= 0; i--)
        cout << c[i];
    cout << "\n";

    return 0;
}
