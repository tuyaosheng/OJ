#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int L, R, d;
    cin >> L >> R >> d;
    long long cnt = 0;
    for (int x = L; x <= R; x++)
    {
        int t = x;
        while (t > 0)                // 拆位：反复取个位
        {
            if (t % 10 == d) cnt++;
            t /= 10;
        }
        if (x == 0 && d == 0) cnt++; // x=0 特例（本题 L>=1 用不到，保险）
    }
    cout << cnt << "\n";
    return 0;
}
