#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    int n;
    cin >> n;
    long long x;
    cin >> x;
    long long cur = x, ans = x;      // 用第一个元素初始化，处理全负数据
    for (int i = 2; i <= n; i++)
    {
        cin >> x;
        cur = max(x, cur + x);       // 要么另起一段，要么接在前面
        ans = max(ans, cur);         // 更新全局最大
    }
    cout << ans << endl;
    return 0;
}
