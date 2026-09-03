#include <cstdio>
#include <cstring>
#include <queue>

const int MAXN = 1005;
char grid[MAXN][MAXN];
int dist[MAXN][MAXN];
int n, m;
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%s", grid[i] + 1);
    int sx, sy, tx, ty;
    scanf("%d %d %d %d", &sx, &sy, &tx, &ty);

    memset(dist, -1, sizeof(dist));
    std::queue<std::pair<int, int>> q;
    dist[sx][sy] = 0;
    q.push({sx, sy});

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        if (cx == tx && cy == ty) break;   // 第一次到终点，就是最短步数，可以提前退出
        for (int d = 0; d < 4; d++) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
            if (grid[nx][ny] != '.') continue;
            if (dist[nx][ny] != -1) continue;   // 已经访问过，不会有更短的路
            dist[nx][ny] = dist[cx][cy] + 1;
            q.push({nx, ny});
        }
    }

    printf("%d\n", dist[tx][ty]);
    return 0;
}
