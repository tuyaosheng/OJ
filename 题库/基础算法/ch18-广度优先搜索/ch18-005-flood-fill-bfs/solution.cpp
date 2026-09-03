#include <cstdio>
#include <cstring>
#include <queue>

const int MAXN = 1005;
char grid[MAXN][MAXN];
bool visited[MAXN][MAXN];
int n, m;
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

void bfs(int sx, int sy) {
    std::queue<std::pair<int, int>> q;
    visited[sx][sy] = true;   // 入队时立刻标记，避免重复入队
    q.push({sx, sy});
    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        for (int d = 0; d < 4; d++) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
            if (grid[nx][ny] != '.' || visited[nx][ny]) continue;
            visited[nx][ny] = true;
            q.push({nx, ny});
        }
    }
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%s", grid[i] + 1);

    int count = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (grid[i][j] == '.' && !visited[i][j]) {
                count++;
                bfs(i, j);
            }
        }
    }

    printf("%d\n", count);
    return 0;
}
