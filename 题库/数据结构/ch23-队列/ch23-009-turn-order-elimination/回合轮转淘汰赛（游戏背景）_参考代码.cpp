#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <deque>    —— deque

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    deque<int> q;                 // 用 deque 实现队列，方便最后遍历输出
    for (int i = 1; i <= n; i++) q.push_back(i);

    for (int i = 0; i < m; i++)
    {
        int res;
        cin >> res;
        int cur = q.front();
        q.pop_front();
        if (res == 2) q.push_back(cur);   // 存活，回到队尾；阵亡则直接丢弃
    }

    if (q.empty())
    {
        cout << "EMPTY\n";
    }
    else
    {
        bool first = true;
        for (int x : q)
        {
            if (!first) cout << " ";
            cout << x;
            first = false;
        }
        cout << "\n";
    }
    return 0;
}
