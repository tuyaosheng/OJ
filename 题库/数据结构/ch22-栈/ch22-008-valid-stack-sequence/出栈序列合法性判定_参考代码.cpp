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
    vector<int> target(n);
    for (auto &v : target) cin >> v;

    stack<int> st;
    int nxt = 1;                 // 接下来该压哪个数
    bool ok = true;
    for (int x : target)
    {
        if (!st.empty() && st.top() == x)
        {
            st.pop();             // 栈顶恰好就是要的，直接弹出
            continue;
        }
        while (nxt <= n && (st.empty() || st.top() != x))
            st.push(nxt++);       // x 还没入栈，先把它压进来
        if (st.empty() || st.top() != x)   // 压到 n 了还是凑不出 x
        {
            ok = false;
            break;
        }
        st.pop();
    }
    cout << (ok ? "Yes" : "No") << "\n";
    return 0;
}
