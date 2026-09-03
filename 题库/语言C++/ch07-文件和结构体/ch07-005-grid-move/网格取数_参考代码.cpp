#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int g[1005][1005];
// 方向增量：下标 1=上,2=下,3=左,4=右
int dx[5] = {0, -1, 1, 0, 0};
int dy[5] = {0, 0, 0, -1, 1};

int main()
{
    int n, x, y, d;
    cin >> n >> x >> y >> d;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            cin >> g[i][j];
    long long sum = 0;
    // 只要还在网格内，就累加当前格并按方向前进一步
    while (x >= 1 && x <= n && y >= 1 && y <= n)
    {
        sum += g[x][y];
        x += dx[d];
        y += dy[d];
    }
    cout << sum << "\n";
    return 0;
}
