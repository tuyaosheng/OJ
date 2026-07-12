#include <iostream>
using namespace std;

int digit[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};  // 每个数字用的火柴数

// 数 x 需要多少根火柴
int cost(int x)
{
    if (x == 0) return digit[0];
    int c = 0;
    while (x > 0)
    {
        c += digit[x % 10];
        x /= 10;
    }
    return c;
}

int main()
{
    int n;
    cin >> n;
    int total = n - 4;               // 去掉 + 和 = 用的 4 根，剩下给数字
    int ans = 0;
    if (total >= 0)
    {
        // 枚举 A、B，C = A+B；上限 1000 足够（火柴数受限）
        for (int a = 0; a <= 1000; a++)
            for (int b = 0; b <= 1000; b++)
            {
                int c = a + b;
                if (cost(a) + cost(b) + cost(c) == total)
                    ans++;
            }
    }
    cout << ans << endl;
    return 0;
}
