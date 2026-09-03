#include <cstdio>
#include <cstring>
#include <queue>

const int MAXN = 405;
int dist[MAXN][MAXN];
int n, m;
int dx[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dy[8] = {-1, 1, -2, 2, -2, 2, -1, 1};

int main() {
    int sx, sy;
    scanf("%d %d %d %d", &n, &m, &sx, &sy);

    memset(dist, -1, sizeof(dist));
    std::queue<std::pair<int, int>> q;
    dist[sx][sy] = 0;
    q.push({sx, sy});

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        for (int d = 0; d < 8; d++) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
            if (dist[nx][ny] != -1) continue;
            dist[nx][ny] = dist[cx][cy] + 1;
            q.push({nx, ny});
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            printf("%d%c", dist[i][j], j == m ? '\n' : ' ');
        }
    }
    return 0;
}
