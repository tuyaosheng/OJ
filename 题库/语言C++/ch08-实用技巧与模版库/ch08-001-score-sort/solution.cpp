#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

struct Student {
    string name;
    int score;
};

// 分数降序；分数相同按名字字典序升序
bool cmp(const Student &a, const Student &b)
{
    if (a.score != b.score) return a.score > b.score;
    return a.name < b.name;          // string 直接比字典序
}

int main()
{
    ios::sync_with_stdio(false);     // 加速 cin/cout，应对 10 万数据
    int n;
    cin >> n;
    Student a[100005];
    for (int i = 0; i < n; i++)
        cin >> a[i].name >> a[i].score;
    sort(a, a + n, cmp);             // STL 排序，O(n log n)
    for (int i = 0; i < n; i++)
        cout << a[i].name << " " << a[i].score << "\n";
    return 0;
}
