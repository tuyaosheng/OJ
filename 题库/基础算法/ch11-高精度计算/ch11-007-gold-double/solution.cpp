// 金币翻倍：求 2^n 的精确值
// 考点：高精度乘单精度（乘 2 做 n 次）、倒序存储、进位
#include <iostream>
using namespace std;

int a[350];   // 倒序存：a[0] 是个位。2^1000 有 302 位，350 足够

int main()
{
    int n;
    cin >> n;
    a[0] = 1;             // 第 0 天：1 枚金币
    int len = 1;
    for (int d = 0; d < n; d++) {          // 翻倍 n 次
        for (int i = 0; i < len; i++)
            a[i] *= 2;                     // 每一位都乘 2
        for (int i = 0; i < len; i++) {    // 统一进位
            a[i + 1] += a[i] / 10;
            a[i] %= 10;
        }
        if (a[len] > 0) len++;             // 乘 2 最多多出一位
    }
    for (int i = len - 1; i >= 0; i--)     // 从最高位倒着输出
        cout << a[i];
    cout << endl;
    return 0;
}
