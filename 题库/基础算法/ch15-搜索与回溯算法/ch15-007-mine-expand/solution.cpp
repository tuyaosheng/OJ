// 扫雷·自动展开
// 考点：洪水填充变式（0 格扩展、数字格封边）、vis 永不撤销
#include <iostream>
using namespace std;

int n, m, cnt;
char g[105][105];
bool vis[105][105];
int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

// 数 (x,y) 周围 8 格的雷数
int mines(int x, int y)
{
    int c = 0;
    for (int d = 0; d < 8; d++) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
        if (g[nx][ny] == '*') c++;
    }
    return c;
}

void dfs(int x, int y)
{
    if (x < 0 || x >= n || y < 0 || y >= m) return;   // 越界
    if (g[x][y] == '*' || vis[x][y]) return;          // 雷 / 已翻开
    vis[x][y] = true;                                 // 翻开：永久事实，绝不撤销
    cnt++;
    if (mines(x, y) > 0) return;                      // 数字格是"边界"，不再扩展
    for (int d = 0; d < 8; d++)                       // 0 格向 8 方向蔓延
        dfs(x + dx[d], y + dy[d]);
}

int main()
{
    cin >> n >> m;
    for (int i = 0; i < n; i++) cin >> g[i];
    int x, y;
    cin >> x >> y;
    dfs(x - 1, y - 1);      // 转成 0 下标
    cout << cnt << endl;
    return 0;
}
