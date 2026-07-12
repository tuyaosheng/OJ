#include <iostream>
using namespace std;

bool door[10005];            // false=关, true=开；全局数组自动清零

int main()
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++)          // 第 i 个服务员
        for (int j = i; j <= n; j += i)   // i 的倍数：i, 2i, 3i, ...
            door[j] = !door[j];           // 反向处理：开关互换
    bool first = true;
    for (int i = 1; i <= n; i++)
    {
        if (door[i])
        {
            if (!first) cout << " ";
            cout << i;
            first = false;
        }
    }
    cout << endl;
    return 0;
}
