// 迷宫路径计数：从起点走到终点，四方向移动，格子不能重复经过，统计路径数。
//
// 这是回溯的经典战场。核心是 vis 数组的含义：
//   vis[i][j] = 格子 (i,j) 是否在【我当前正在走的这条路径】上。
//
// 所以退回来的时候【必须】把标记擦掉 —— 因为离开这个格子后，
// 它就不再属于当前路径了。漏掉这一步，会错失所有经过该格子的其他路径。
//
// 口诀：进来画叉，出去擦掉。
#include <iostream>
using namespace std;

const int N = 10;

int n, m, sx, sy, tx, ty;
int g[N][N];            // 迷宫：0 空地，1 障碍
bool vis[N][N];         // 该格子是否在当前路径上
int ans = 0;

// 四个方向的偏移：上、下、左、右
// 用方向数组比写四个 if 优雅得多，是网格搜索的标准写法
int dx[4] = {-1, 1, 0, 0};
int dy[4] = { 0, 0,-1, 1};

void dfs(int x, int y)
{
    // 到达终点，找到一条完整路径
    if (x == tx && y == ty)
    {
        ans++;
        return;         // 到了就别再往下走了
    }

    // 尝试四个方向
    for (int d = 0; d < 4; d++)
    {
        int nx = x + dx[d];
        int ny = y + dy[d];

        if (nx < 1 || nx > n || ny < 1 || ny > m) continue;  // 出界
        if (g[nx][ny] == 1) continue;                        // 障碍
        if (vis[nx][ny]) continue;                           // 已在当前路径上，不能重复经过

        vis[nx][ny] = true;     // ① 做选择：踏上这个格子，标记它在路径上
        dfs(nx, ny);            // ② 递归：从新位置继续探索
        vis[nx][ny] = false;    // ③ ★撤销★ 退回来了，这个格子不再属于当前路径
        // 漏掉这一行 => 走别的岔路时会误以为该格子被占用 => 大量路径丢失
    }
}

int main()
{
    cin >> n >> m;
    cin >> sx >> sy >> tx >> ty;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            cin >> g[i][j];

    vis[sx][sy] = true;     // 别忘了标记起点，否则路径可能绕回起点
    dfs(sx, sy);

    cout << ans << "\n";

    return 0;
}
