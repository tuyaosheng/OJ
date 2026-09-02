// 出现次数最多的数（众数）：排序 + 扫描。
// 核心思想：排序会把相同的数聚成连续的一"块"，于是"计数"问题变成"数每块有多长"。
// 数值范围 ±10^9，没法开桶数组计数，所以用排序。
// 复杂度：O(n log n)。
#include <iostream>
#include <algorithm>
using namespace std;

const int N = 100005;

int a[N];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 0; i < n; i++)
        cin >> a[i];

    // 关键一步：排序后，所有相同的数都挨在一起了
    sort(a, a + n);

    int bestVal = a[0], bestCnt = 1;    // 目前找到的最优答案
    int curVal = a[0], curCnt = 1;      // 当前这一"块"的值和长度

    for (int i = 1; i < n; i++)
    {
        if (a[i] == curVal)
        {
            curCnt++;               // 还在同一块里，长度 +1
        }
        else
        {
            curVal = a[i];          // 遇到新的数，开启新的一块
            curCnt = 1;
        }

        // 注意这里必须是【严格大于】，不能是 >=
        // 因为数组从小到大扫描，小的数先被处理；
        // 后面出现次数相同的（更大的）数不会满足 >，于是不会覆盖答案。
        // 这样就自动保证了"并列时取最小的那个"。
        if (curCnt > bestCnt)
        {
            bestCnt = curCnt;
            bestVal = curVal;
        }
    }

    cout << bestVal << " " << bestCnt << "\n";

    return 0;
}
