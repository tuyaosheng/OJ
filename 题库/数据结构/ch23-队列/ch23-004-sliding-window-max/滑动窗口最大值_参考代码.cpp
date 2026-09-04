#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <deque>    —— deque

int a[200005];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    cin >> n >> k;
    for (int i = 0; i < n; i++) cin >> a[i];

    deque<int> dq;                // 存下标，值从队首到队尾严格递减
    vector<int> ans;
    for (int i = 0; i < n; i++)
    {
        while (!dq.empty() && a[dq.back()] <= a[i])
            dq.pop_back();         // 队尾这些已经不可能是最大值了
        dq.push_back(i);

        if (dq.front() <= i - k)   // 队首滑出了窗口
            dq.pop_front();

        if (i >= k - 1)             // 窗口已填满
            ans.push_back(a[dq.front()]);
    }

    for (size_t i = 0; i < ans.size(); i++)
    {
        if (i > 0) cout << " ";
        cout << ans[i];
    }
    cout << "\n";
    return 0;
}
