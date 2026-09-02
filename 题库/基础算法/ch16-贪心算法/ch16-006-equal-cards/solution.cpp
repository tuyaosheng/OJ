// 均分纸牌
// 考点：前缀和贪心
// 结论：答案 = 前 n-1 个"与平均值之差"的前缀和的绝对值之和
#include <cstdio>
#include <cstdlib>
using namespace std;

long long a[100005];

int main()
{
    int n;
    scanf("%d", &n);
    long long sum = 0;
    for (int i = 0; i < n; i++) { scanf("%lld", &a[i]); sum += a[i]; }
    long long avg = sum / n;

    long long s = 0, ans = 0;
    for (int i = 0; i < n - 1; i++) {   // 相邻间隔只有 n-1 个
        s += a[i] - avg;                // 累计到第 i 堆为止，比"应有总量"多/少多少
        ans += llabs(s);                // 这部分差额必须跨过第 i/第 i+1 堆之间的间隔
    }
    printf("%lld\n", ans);
    return 0;
}
