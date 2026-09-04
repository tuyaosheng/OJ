#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <string>   —— string
//   <stack>    —— stack

int prec(char op)   // 运算符优先级：* / 高于 + -
{
    if (op == '*' || op == '/') return 2;
    return 1;                    // + 或 -
}

int main()
{
    string s;
    cin >> s;
    int n = s.size();
    stack<char> opSt;             // 运算符栈
    vector<string> out;           // 输出的 token 序列

    int i = 0;
    while (i < n)
    {
        if (isdigit(s[i]))
        {
            int j = i;
            while (j < n && isdigit(s[j])) j++;   // 把连续数字合并成一个 token
            out.push_back(s.substr(i, j - i));
            i = j;
        }
        else if (s[i] == '(')
        {
            opSt.push(s[i]);
            i++;
        }
        else if (s[i] == ')')
        {
            while (opSt.top() != '(')
            {
                out.push_back(string(1, opSt.top()));
                opSt.pop();
            }
            opSt.pop();           // 丢弃这个 '('
            i++;
        }
        else                      // + - * /
        {
            char op = s[i];
            // 栈顶优先级 >= 当前运算符时，先弹出输出（>= 保证左结合）
            while (!opSt.empty() && opSt.top() != '(' && prec(opSt.top()) >= prec(op))
            {
                out.push_back(string(1, opSt.top()));
                opSt.pop();
            }
            opSt.push(op);
            i++;
        }
    }
    while (!opSt.empty())         // 扫描结束，栈里剩的运算符全部弹出
    {
        out.push_back(string(1, opSt.top()));
        opSt.pop();
    }

    for (size_t k = 0; k < out.size(); k++)
    {
        if (k > 0) cout << " ";
        cout << out[k];
    }
    cout << "\n";
    return 0;
}
