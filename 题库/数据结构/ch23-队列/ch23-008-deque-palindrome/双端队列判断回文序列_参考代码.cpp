#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <deque>    —— deque

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    deque<int> dq;
    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        dq.push_back(x);
    }

    bool ok = true;
    while (dq.size() > 1)
    {
        if (dq.front() != dq.back())   // 两端不相等，不是回文
        {
            ok = false;
            break;
        }
        dq.pop_front();
        dq.pop_back();
    }
    cout << (ok ? "Yes" : "No") << "\n";
    return 0;
}
