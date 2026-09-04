#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <stack>    —— stack

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    stack<int> in, out;         // in 接新元素，out 负责吐出队首
    while (n--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            int x;
            cin >> x;
            in.push(x);
        }
        else                     // op == 2：出队
        {
            if (out.empty())      // out 空了才把 in 整体倒过去（顺序刚好反过来）
            {
                while (!in.empty())
                {
                    out.push(in.top());
                    in.pop();
                }
            }
            cout << out.top() << "\n";
            out.pop();
        }
    }
    return 0;
}
