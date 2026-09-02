// 排序不等式·最优配对
// 考点：排序不等式（乱序和 <= 乱序和 <= 顺序和 的最小情形）
// 结论：一个升序、一个降序配对，乘积和最小
#include <cstdio>
#include <algorithm>
#include <functional>
using namespace std;

int a[1005], b[1005];

int main()
{
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    for (int i = 0; i < n; i++) scanf("%d", &b[i]);

    sort(a, a + n);                      // a 升序
    sort(b, b + n, greater<int>());      // b 降序：大的配小的

    long long ans = 0;
    for (int i = 0; i < n; i++) ans += (long long)a[i] * b[i];
    printf("%lld\n", ans);
    return 0;
}
