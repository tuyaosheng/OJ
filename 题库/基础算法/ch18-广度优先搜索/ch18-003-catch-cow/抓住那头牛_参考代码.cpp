#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <queue>    —— queue
//   <cstring>  —— memset

const int MAXX = 100005;
int dist[MAXX];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    cin >> n >> k;

    memset(dist, -1, sizeof(dist));
    queue<int> q;
    dist[n] = 0;
    q.push(n);

    while (!q.empty()) {
        int x = q.front();
        q.pop();
        if (x == k) break;   // 第一次到达 k，就是最少分钟数

        int next[3] = {x - 1, x + 1, x * 2};
        for (int i = 0; i < 3; i++) {
            int nx = next[i];
            if (nx < 0 || nx > 100000) continue;
            if (dist[nx] != -1) continue;
            dist[nx] = dist[x] + 1;
            q.push(nx);
        }
    }

    cout << dist[k] << "\n";
    return 0;
}
