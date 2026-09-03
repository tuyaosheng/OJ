#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
// 考点：if - else if 链、分段判断、边界值

int main()
{
    int v;
    cin >> v;
    // 从小到大依次判断，每个 else if 隐含"前面的都不满足"
    if (v < 100)        cout << "白色" << "\n";
    else if (v < 300)   cout << "绿色" << "\n";
    else if (v < 600)   cout << "蓝色" << "\n";
    else if (v < 1000)  cout << "紫色" << "\n";
    else                cout << "橙色" << "\n";
    return 0;
}
