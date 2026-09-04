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
    stack<int> st;
    while (n--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            int x;
            cin >> x;
            st.push(x);          // 入栈
        }
        else if (op == 2)
        {
            cout << st.top() << "\n";  // pop() 不返回值，要先 top() 记下来
            st.pop();
        }
        else                     // op == 3
        {
            cout << st.size() << "\n";
        }
    }
    return 0;
}
