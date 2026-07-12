#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

struct Student {           // 结构体：把一个学生的信息打包
    string name;
    int total;             // 总分 = 语文 + 数学
};

// 比较规则：总分高的排前面
bool cmp(const Student &a, const Student &b)
{
    return a.total > b.total;
}

int main()
{
    int n;
    cin >> n;
    Student s[105];
    for (int i = 0; i < n; i++)
    {
        int chinese, math;
        cin >> s[i].name >> chinese >> math;
        s[i].total = chinese + math;
    }
    // stable_sort 稳定排序：总分相同者保持输入先后
    stable_sort(s, s + n, cmp);
    for (int i = 0; i < n; i++)
        cout << s[i].name << " " << s[i].total << endl;
    return 0;
}
