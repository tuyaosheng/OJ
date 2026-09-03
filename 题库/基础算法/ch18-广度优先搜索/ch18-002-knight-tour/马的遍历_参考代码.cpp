#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <queue>    —— queue
//   <cstring>  —— memset

const int MAXN = 405;
int dist[MAXN][MAXN];   // dist[x][y] = 从起点到 (x,y) 最少跳几步马；-1 表示还没到过
int n, m;
int dx[8] = {-2, -2, -1, -1, 1, 1, 2, 2};   // 马走"日"字的 8 种偏移
int dy[8] = {-1, 1, -2, 2, -2, 2, -1, 1};

int main() {
    ios::sync_with_stdio(false);   // 关闭和 C 标准库的同步，加速 cin/cout 在大数据下的输入
    cin.tie(nullptr);
    int sx, sy;
    cin >> n >> m >> sx >> sy;

    memset(dist, -1, sizeof(dist));
    queue<pair<int, int>> q;
    dist[sx][sy] = 0;
    q.push({sx, sy});

    // 单源 BFS：按"层"扩展，第一次到达某格时记录的步数一定是最短的，无需再更新
    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        for (int d = 0; d < 8; d++) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
            if (dist[nx][ny] != -1) continue;   // 已访问过，跳过（BFS 保证首次即最短）
            dist[nx][ny] = dist[cx][cy] + 1;
            q.push({nx, ny});
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cout << dist[i][j] << (j == m ? '\n' : ' ');
        }
    }
    return 0;
}
