#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
// 考点：二维字符数组、8 邻域枚举、越界判断

char g[105][105];

int main()
{
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; i++) cin >> g[i];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (g[i][j] == '*') { cout << '*'; continue; }
            int cnt = 0;
            // 枚举 8 个方向的偏移量
            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;    // 自己不算
                    int ni = i + dx, nj = j + dy;
                    if (ni < 0 || ni >= n || nj < 0 || nj >= m) continue;  // 越界跳过
                    if (g[ni][nj] == '*') cnt++;
                }
            cout << cnt;
        }
        cout << "\n";
    }
    return 0;
}
