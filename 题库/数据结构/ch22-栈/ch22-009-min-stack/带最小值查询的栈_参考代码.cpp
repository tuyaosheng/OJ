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
    stack<long long> st, minSt;   // minSt 栈顶始终是主栈当前的最小值
    while (n--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            long long x;
            cin >> x;
            st.push(x);
            if (minSt.empty() || x <= minSt.top())
                minSt.push(x);         // 新的最小值
            else
                minSt.push(minSt.top()); // 最小值不变，复制一份保持同步
        }
        else if (op == 2)
        {
            st.pop();
            minSt.pop();               // 两栈同步弹出
        }
        else                          // op == 3
        {
            cout << minSt.top() << "\n";
        }
    }
    return 0;
}
