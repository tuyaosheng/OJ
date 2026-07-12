#include <iostream>
using namespace std;

int main()
{
    int L, R, cnt = 0;           // cnt 统计输出了几个素数
    cin >> L >> R;
    for (int i = L; i <= R; i++)
    {
        bool isPrime = true;     // 先假设 i 是素数
        for (int j = 2; j * j <= i; j++)  // 只需试除到 sqrt(i)
        {
            if (i % j == 0)      // 找到约数，不是素数
            {
                isPrime = false;
                break;           // 提前退出内层循环
            }
        }
        if (isPrime)
        {
            cout << i << endl;
            cnt++;
        }
    }
    if (cnt == 0)                // 区间内一个素数都没有
        cout << -1 << endl;
    return 0;
}
