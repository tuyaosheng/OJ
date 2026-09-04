#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <string>   —— string
//   <stack>    —— stack
//   <cctype>   —— toupper

int main()
{
    string s;
    cin >> s;
    stack<char> st;
    bool ok = true;
    for (char c : s)
    {
        if (isupper(c))
        {
            st.push(c);           // 触发：入栈
        }
        else                      // 复位（小写字母）
        {
            if (st.empty() || st.top() != toupper(c))
            {
                ok = false;
                break;
            }
            st.pop();              // 类型对得上，弹栈
        }
    }
    if (!st.empty()) ok = false;    // 还有触发的机关没复位
    cout << (ok ? "Yes" : "No") << "\n";
    return 0;
}
