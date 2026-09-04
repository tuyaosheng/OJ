#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <stack>    —— stack

int a[200005];
int ans[200005];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    stack<int> st;                // 存下标，值从栈底到栈顶严格递减
    for (int i = 1; i <= n; i++)
    {
        while (!st.empty() && a[st.top()] < a[i])
        {
            ans[st.top()] = a[i];  // a[i] 就是栈顶下标右边第一个更大的元素
            st.pop();
        }
        st.push(i);
    }
    while (!st.empty())           // 栈里剩下的，右边没有更大的元素
    {
        ans[st.top()] = -1;
        st.pop();
    }

    for (int i = 1; i <= n; i++)
    {
        if (i > 1) cout << " ";
        cout << ans[i];
    }
    cout << "\n";
    return 0;
}
