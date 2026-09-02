#include <iostream>
#include <set>
using namespace std;

int main()
{
    int n;
    cin >> n;
    set<int> s;                      // 有序集合：自动去重 + 自动排序
    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        s.insert(x);
    }
    cout << s.size() << endl;        // 去重后的个数
    bool first = true;
    for (int x : s)                  // 遍历 set 即从小到大
    {
        if (!first) cout << " ";
        cout << x;
        first = false;
    }
    cout << endl;
    return 0;
}
