// A-B 数对：统计满足 a[i] - a[j] = C 的下标对 (i, j) 个数。
// 核心变形：a[i] - a[j] = C  <=>  a[i] = a[j] + C
//   于是枚举 j，数出"数组里等于 a[j]+C 的元素有多少个"，累加即可。
// 快速计数：排序后相同的数连成一段，用 upper_bound - lower_bound 求出段长。
// 复杂度：O(n log n)。暴力两重循环是 O(n^2) = 100 亿，必然超时。
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
    long long C;
    cin >> n >> C;

    for (int i = 0; i < n; i++)
        cin >> a[i];

    // 二分的前提：必须先排序
    sort(a, a + n);

    long long ans = 0;      // 答案可能达到 n^2 = 10^10，必须用 long long！

    for (int j = 0; j < n; j++)
    {
        // 防溢出：a[j] 最大 1e9，C 最大 1e9，相加接近/超过 int 上限，先转 long long
        long long target = (long long)a[j] + C;

        // 等于 target 的元素个数 = 第一个 >target 的位置 - 第一个 >=target 的位置
        int cnt = upper_bound(a, a + n, target) - lower_bound(a, a + n, target);

        ans += cnt;
    }
    // 因为 C >= 1，所以 a[i] = a[j] + C 保证 a[i] != a[j]，
    // i 和 j 天然不会是同一个下标，无需额外排除。

    cout << ans << "\n";

    return 0;
}
