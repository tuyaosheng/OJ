// 排队接水
// 考点：排序贪心 + 交换论证
// 结论：打水时间短的先打，平均等待时间最短
#include <cstdio>
#include <algorithm>
using namespace std;

int t[1005];

int main()
{
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &t[i]);
    sort(t, t + n);                 // 从小到大排序：贪心的核心一步

    long long total = 0, wait = 0;  // wait: 目前为止排在前面的人共用掉多少时间
    for (int i = 0; i < n; i++) {
        total += wait;               // 当前这位同学要等这么久
        wait += t[i];                // 他打完后，后面的人又要多等这么久
    }
    printf("%.2f\n", (double)total / n);
    return 0;
}
