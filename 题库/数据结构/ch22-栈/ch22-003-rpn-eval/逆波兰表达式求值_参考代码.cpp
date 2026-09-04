#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout / getline
//   <sstream>  —— stringstream
//   <string>   —— string
//   <stack>    —— stack

int main()
{
    string line;
    getline(cin, line);
    stringstream ss(line);
    string tok;
    stack<long long> st;
    while (ss >> tok)
    {
        // 运算符 token 长度恒为 1；负数 token（如 "-3"）长度至少为 2，据此区分
        if (tok.size() == 1 && (tok[0] == '+' || tok[0] == '-' || tok[0] == '*' || tok[0] == '/'))
        {
            long long b = st.top(); st.pop();   // 先弹出的是右操作数
            long long a = st.top(); st.pop();   // 后弹出的是左操作数
            long long res = 0;
            if (tok[0] == '+') res = a + b;
            else if (tok[0] == '-') res = a - b;
            else if (tok[0] == '*') res = a * b;
            else res = a / b;                   // 数据保证能整除
            st.push(res);
        }
        else
        {
            st.push(stoll(tok));                // 数字 token 直接转整数入栈
        }
    }
    cout << st.top() << "\n";
    return 0;
}
