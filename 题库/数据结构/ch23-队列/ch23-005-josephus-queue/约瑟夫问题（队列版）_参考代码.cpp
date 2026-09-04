#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <queue>    —— queue

int main()
{
    int n, k;
    cin >> n >> k;
    queue<int> q;
    for (int i = 1; i <= n; i++) q.push(i);

    vector<int> ans;
    while (!q.empty())
    {
        for (int i = 1; i < k; i++)   // 轮转 k-1 次：报数但不出列
        {
            q.push(q.front());
            q.pop();
        }
        ans.push_back(q.front());     // 第 k 个，真正出列
        q.pop();
    }

    for (size_t i = 0; i < ans.size(); i++)
    {
        if (i > 0) cout << " ";
        cout << ans[i];
    }
    cout << "\n";
    return 0;
}
