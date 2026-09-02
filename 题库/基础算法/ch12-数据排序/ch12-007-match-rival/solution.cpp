// 匹配对手
// 考点：排序 + lower_bound 二分、边界处理
#include <iostream>
#include <algorithm>
using namespace std;

int b[100005];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) cin >> b[i];
    sort(b, b + n);                        // 先排序，二分的前提！

    int m;
    cin >> m;
    while (m--) {
        int x;
        cin >> x;
        int p = lower_bound(b, b + n, x) - b;   // 第一个 >= x 的位置
        long long ans = 4000000000LL;           // 比一切可能的差都大
        if (p < n) ans = min(ans, (long long)b[p] - x);       // 右侧候选
        if (p > 0) ans = min(ans, (long long)x - b[p - 1]);   // 左侧候选
        cout << ans << "\n";
    }
    return 0;
}
