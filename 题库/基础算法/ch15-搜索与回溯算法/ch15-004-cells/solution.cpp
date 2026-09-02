// 细胞个数（连通块计数 / 洪水填充 Flood Fill）
//
// 思路：扫描每个格子，一旦发现"非 0 且还没被访问过"的格子，
//   说明发现了一个【新细胞】=> 计数 +1，然后用 DFS 把与它连通的所有格子全部标记掉。
//   这样同一个细胞的其他格子，之后就不会被再次当成新细胞。
//
// ★★★ 关键：这题【绝对不能撤销】vis 标记！★★★
//   对比上一题（迷宫路径计数）：
//     迷宫的 vis = "在当前路径上"  -> 离开就不在路径上了 -> 必须撤销
//     本题的 vis = "曾经被访问过"  -> 永远属于那个细胞   -> 绝不撤销
//   若错误地撤销：同一细胞会被反复计数，且 DFS 会 A->B->A 来回横跳，无限递归爆栈。
//
// 复杂度：每个格子只访问一次，O(n*m)。
#include <iostream>
#include <string>
using namespace std;

const int N = 105;

int n, m;
int g[N][N];            // 0 = 空白，非 0 = 细胞组织
bool vis[N][N];         // 该格子是否【曾经被访问过】

int dx[4] = {-1, 1, 0, 0};
int dy[4] = { 0, 0,-1, 1};

// 把与 (x,y) 连通的所有细胞格子，全部"染色"（标记为已访问）
void dfs(int x, int y)
{
    vis[x][y] = true;               // 标记：这个格子归入当前细胞

    for (int d = 0; d < 4; d++)     // 向四个方向蔓延
    {
        int nx = x + dx[d];
        int ny = y + dy[d];

        if (nx < 1 || nx > n || ny < 1 || ny > m) continue;  // 出界
        if (g[nx][ny] == 0) continue;                        // 空白，不属于细胞
        if (vis[nx][ny]) continue;                           // 已标记过

        dfs(nx, ny);                // 继续向外蔓延
    }

    // ★ 这里【没有】vis[x][y] = false —— 绝不能撤销！
    // 标记要永久保留，才能保证每个格子只被数一次。
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    // 每行是一个数字串（中间没空格），用 string 读入
    for (int i = 1; i <= n; i++)
    {
        string s;
        cin >> s;
        for (int j = 0; j < m; j++)
            g[i][j + 1] = s[j] - '0';       // 字符转数字
    }

    int ans = 0;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
        {
            // 发现一个"非 0 且没访问过"的格子 => 这是一个全新的细胞
            if (g[i][j] != 0 && !vis[i][j])
            {
                ans++;          // 细胞数 +1
                dfs(i, j);      // 把这整个细胞的所有格子一次性标记掉
            }
        }

    cout << ans << "\n";

    return 0;
}
