#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— sort

struct Stu {
    int id;        // 学号（输入顺序）
    int chinese;   // 语文
    int total;     // 总分
};

// 多关键字：总分降序 > 语文降序 > 学号升序
bool cmp(const Stu &a, const Stu &b)
{
    if (a.total != b.total)     return a.total > b.total;
    if (a.chinese != b.chinese) return a.chinese > b.chinese;
    return a.id < b.id;
}

int main()
{
    int n;
    cin >> n;
    Stu s[305];
    for (int i = 0; i < n; i++)
    {
        int c, m, e;
        cin >> c >> m >> e;
        s[i].id = i + 1;          // 学号从 1 开始
        s[i].chinese = c;
        s[i].total = c + m + e;
    }
    sort(s, s + n, cmp);
    for (int i = 0; i < 5; i++)   // 输出前 5 名
        cout << s[i].id << " " << s[i].total << "\n";
    return 0;
}
