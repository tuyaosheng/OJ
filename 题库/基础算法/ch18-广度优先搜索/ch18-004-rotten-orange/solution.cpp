#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>

const int MAXN = 1005;
int grid[MAXN][MAXN];
int dist[MAXN][MAXN];
int n, m;
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int main() {
    scanf("%d %d", &n, &m);
    int freshCount = 0;
    std::queue<std::pair<int, int>> q;
    memset(dist, -1, sizeof(dist));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%d", &grid[i][j]);
            if (grid[i][j] == 1) freshCount++;
            if (grid[i][j] == 2) {
                dist[i][j] = 0;      // 所有初始烂橘子同时是第 0 分钟
                q.push({i, j});      // 多源 BFS：一次性把所有起点入队
            }
        }
    }

    if (freshCount == 0) {
        printf("0\n");
        return 0;
    }

    int maxTime = 0;
    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        for (int d = 0; d < 4; d++) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
            if (grid[nx][ny] != 1) continue;   // 只有新鲜橘子会被感染
            if (dist[nx][ny] != -1) continue;
            dist[nx][ny] = dist[cx][cy] + 1;
            maxTime = std::max(maxTime, dist[nx][ny]);
            freshCount--;
            q.push({nx, ny});
        }
    }

    printf("%d\n", freshCount == 0 ? maxTime : -1);
    return 0;
}
