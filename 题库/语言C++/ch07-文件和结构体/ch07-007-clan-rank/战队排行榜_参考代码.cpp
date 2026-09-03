#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <string>    —— string
//   <algorithm> —— sort
// 考点：结构体、sort 多关键字排序

struct Team {
    string name;
    int score, time;
};

Team t[1005];

// 多关键字：积分降序 -> 用时升序 -> 队名字典序升序
bool cmp(const Team &a, const Team &b)
{
    if (a.score != b.score) return a.score > b.score;
    if (a.time != b.time) return a.time < b.time;
    return a.name < b.name;
}

int main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> t[i].name >> t[i].score >> t[i].time;
    sort(t, t + n, cmp);
    for (int i = 0; i < n; i++)
        cout << t[i].name << " " << t[i].score << " " << t[i].time << "\n";
    return 0;
}
