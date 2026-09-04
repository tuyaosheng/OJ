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
    while (n--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            int x;
            cin >> x;
            dq.push_back(x);
        }
        else if (op == 2)
        {
            int x;
            cin >> x;
            dq.push_front(x);
        }
        else if (op == 3)
        {
            cout << dq.back() << "\n";
            dq.pop_back();
        }
        else                      // op == 4
        {
            cout << dq.front() << "\n";
            dq.pop_front();
        }
    }
    return 0;
}
