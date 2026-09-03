#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <string>   —— string
//
// 单词方阵：在 n×n 字母方阵中找出所有 8 个方向【直线】排列的 "yizhong"，
//           保留这些字母，其余位置输出 '*'。
//
// ★ 关键理解：这【不是】自由乱走的搜索！
//   迷宫题里每步可任选方向（路径能拐弯）；
//   但本题单词必须【直线】排列 —— 一旦选定方向，就得一直沿它走到底，不能变向。
//   实现要点：nx = x + dx[d] * k —— 方向 d 固定，只有步数 k 在变。
//
// ★ 另一个要点：必须【先全部找完，再统一输出】。
//   因为一个字母可能同时属于多个交叉的 yizhong，
//   边找边改方阵会破坏还没检查的字母。
//
// 复杂度：O(n^2 * 8 * 7)，n=100 时约 56 万次。

const int N = 105;
const string word = "yizhong";      // 目标单词，共 7 个字母

int n;
char g[N][N];
bool keep[N][N];        // 该格子是否属于某个 yizhong（要保留）

// 8 个方向：把 (-1,0,1) x (-1,0,1) 的 9 种组合去掉 (0,0)
int dx[8] = {-1,-1,-1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1,-1, 1,-1, 0, 1};

// 从 (x,y) 出发，沿【固定方向 d】直线前进，检查能否拼出完整的 word
bool check(int x, int y, int d)
{
    for (int k = 0; k < (int)word.size(); k++)
    {
        // ★ 方向 d 固定，只有步数 k 在变 => 保证走的是直线
        int nx = x + dx[d] * k;
        int ny = y + dy[d] * k;

        if (nx < 1 || nx > n || ny < 1 || ny > n) return false;  // 走出方阵
        if (g[nx][ny] != word[k]) return false;                  // 字母对不上
    }

    return true;        // 7 个字母全部匹配成功
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    for (int i = 1; i <= n; i++)
    {
        string s;
        cin >> s;
        for (int j = 1; j <= n; j++)
            g[i][j] = s[j - 1];
    }

    // 枚举每个起点、每个方向
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            for (int d = 0; d < 8; d++)
                if (check(i, j, d))
                {
                    // 找到一个 yizhong，把它的 7 个格子都标记为"保留"
                    for (int k = 0; k < (int)word.size(); k++)
                        keep[i + dx[d] * k][j + dy[d] * k] = true;
                    // 注意：这里不撤销！keep 的语义是"永久保留"，
                    // 和"细胞个数"一样，标记一旦打上就永久有效。
                }

    // 全部搜完之后，才统一输出（一个字母可能属于多个交叉的单词）
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            cout << (keep[i][j] ? g[i][j] : '*');
        cout << "\n";
    }

    return 0;
}
