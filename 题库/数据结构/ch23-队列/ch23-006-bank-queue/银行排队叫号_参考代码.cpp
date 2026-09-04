#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <queue>    —— queue
//   <vector>   —— vector

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<queue<int>> q(m + 1);   // q[1..m] 对应 m 个窗口
    int nextId = 1;                 // 下一位到达顾客的编号

    while (n--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            int best = 1;
            for (int i = 2; i <= m; i++)      // 找排队人数最少（并列取编号小）的窗口
                if (q[i].size() < q[best].size())
                    best = i;
            q[best].push(nextId++);
        }
        else                      // op == 2，格式 "2 w"
        {
            int w;
            cin >> w;
            cout << q[w].front() << "\n";
            q[w].pop();
        }
    }
    return 0;
}
