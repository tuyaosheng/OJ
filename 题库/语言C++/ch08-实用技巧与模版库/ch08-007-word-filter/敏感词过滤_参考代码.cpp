#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <string>   —— string
// 考点：string 的 find / replace、循环查找

int main()
{
    string s, w;
    cin >> s >> w;
    size_t p = s.find(w);                  // 找不到时返回 string::npos
    while (p != string::npos) {
        s.replace(p, w.size(), string(w.size(), '*'));  // 换成等长的 *
        p = s.find(w, p + w.size());       // 从被替换段之后继续找
    }
    cout << s << "\n";
    return 0;
}
