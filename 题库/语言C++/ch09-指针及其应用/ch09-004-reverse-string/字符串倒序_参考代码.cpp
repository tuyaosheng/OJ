#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <cstring>  —— strlen

char s[100005];

int main()
{
    cin >> s;
    int len = strlen(s);
    char *p = s;                     // 指向开头
    char *q = s + len - 1;           // 指向结尾
    while (p < q)                    // 两端向中间靠拢
    {
        char t = *p; *p = *q; *q = t;  // 交换首尾字符
        p++;
        q--;
    }
    cout << s << "\n";
    return 0;
}
