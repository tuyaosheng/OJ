#include <cstdio>
#include <cstring>
#include <queue>

const int MAXN = 1005;
int grid[MAXN][MAXN];
int dist[MAXN][MAXN];
int n, m;
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int main() {
    scanf("%d %d", &n, &m);
    memset(dist, -1, sizeof(dist));
    std::queue<std::pair<int, int>> q;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%d", &grid[i][j]);
            if (grid[i][j] == 2) {
                dist[i][j] = 0;      // 所有刷新点同时是第 0 秒
                q.push({i, j});      // 多源 BFS：一次性入队
            }
        }
    }

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        for (int d = 0; d < 4; d++) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
            if (grid[nx][ny] == 1) continue;      // 障碍物不能走
            if (dist[nx][ny] != -1) continue;
            dist[nx][ny] = dist[cx][cy] + 1;
            q.push({nx, ny});
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int val = (grid[i][j] == 1) ? -1 : dist[i][j];
            printf("%d%c", val, j == m ? '\n' : ' ');
        }
    }
    return 0;
}
