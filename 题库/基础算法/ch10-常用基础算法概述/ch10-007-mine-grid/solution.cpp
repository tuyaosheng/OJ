// 矿区资源统计
// 考点：二维前缀和（建表 + 容斥查询）
#include <iostream>
using namespace std;

long long s[505][505];   // 二维前缀和，全局数组自动清零，s[0][*]=s[*][0]=0

int main()
{
    ios::sync_with_stdio(false);   // 数据量大，关同步加速
    cin.tie(0);
    int n, m, q;
    cin >> n >> m >> q;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            int a;
            cin >> a;
            // 建表容斥：上 + 左 - 左上（重复） + 自己
            s[i][j] = s[i - 1][j] + s[i][j - 1] - s[i - 1][j - 1] + a;
        }
    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        // 查询容斥：大矩形 - 上条 - 左条 + 左上角（多减了补回）
        long long ans = s[x2][y2] - s[x1 - 1][y2] - s[x2][y1 - 1] + s[x1 - 1][y1 - 1];
        cout << ans << "\n";
    }
    return 0;
}
