// 敏感词过滤
// 考点：string 的 find / replace、循环查找
#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s, w;
    cin >> s >> w;
    size_t p = s.find(w);                  // 找不到时返回 string::npos
    while (p != string::npos) {
        s.replace(p, w.size(), string(w.size(), '*'));  // 换成等长的 *
        p = s.find(w, p + w.size());       // 从被替换段之后继续找
    }
    cout << s << endl;
    return 0;
}
