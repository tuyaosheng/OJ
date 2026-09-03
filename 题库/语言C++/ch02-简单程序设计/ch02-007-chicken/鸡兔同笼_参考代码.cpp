#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    long long h, f;              // 头数、脚数（脚数最大 4*10^6，int 也够，用 long long 更保险）
    cin >> h >> f;
    long long t = (f - 2 * h) / 2;   // 假设全是鸡：多出的脚数除以 2 = 兔的只数
    long long j = h - t;             // 鸡 = 总头数 - 兔
    cout << j << " " << t << "\n";
    return 0;
}
