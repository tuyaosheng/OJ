// 本题用到 queue、utility（pair）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <queue> <utility> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXR = 1005;
int R, C;
char grid[MAXR][MAXR];
bool visited_[MAXR][MAXR];
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

bool bfs(int sr, int sc, int tr, int tc) {
    queue<pair<int, int>> q;
    q.push({sr, sc});
    visited_[sr][sc] = true;
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == tr && c == tc) return true;
        for (int d = 0; d < 4; d++) {
            int nr = r + dx[d], nc = c + dy[d];
            if (nr < 1 || nr > R || nc < 1 || nc > C) continue;
            if (grid[nr][nc] == '#' || visited_[nr][nc]) continue;
            visited_[nr][nc] = true;
            q.push({nr, nc});
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> R >> C;
    for (int i = 1; i <= R; i++) {
        string row;
        cin >> row;
        for (int j = 1; j <= C; j++) grid[i][j] = row[j - 1];
    }
    int sr, sc, tr, tc;
    cin >> sr >> sc >> tr >> tc;
    cout << (bfs(sr, sc, tr, tc) ? "Yes" : "No") << '\n';
    return 0;
}
