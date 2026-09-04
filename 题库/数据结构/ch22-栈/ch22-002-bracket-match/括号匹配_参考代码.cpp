#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <string>   —— string
//   <stack>    —— stack

int main()
{
    string s;
    cin >> s;
    stack<char> st;
    bool ok = true;
    for (char c : s)
    {
        if (c == '(' || c == '[' || c == '{')
        {
            st.push(c);          // 左括号直接入栈
        }
        else                     // 右括号：检查栈顶是否是配套的左括号
        {
            if (st.empty())
            {
                ok = false;
                break;
            }
            char top = st.top();
            bool match = (c == ')' && top == '(') ||
                         (c == ']' && top == '[') ||
                         (c == '}' && top == '{');
            if (!match)
            {
                ok = false;
                break;
            }
            st.pop();             // 配上了，弹出
        }
    }
    if (!st.empty()) ok = false;   // 扫完还有左括号没配对
    cout << (ok ? "Yes" : "No") << "\n";
    return 0;
}
