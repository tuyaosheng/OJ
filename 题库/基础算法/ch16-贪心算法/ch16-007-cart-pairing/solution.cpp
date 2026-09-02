// 矿车配对
// 考点：双指针贪心（配对问题）
// 结论：排序后，最重的优先配最轻的；配不上就最重的单独一辆车
#include <cstdio>
#include <algorithm>
using namespace std;

int w[100005];

int main()
{
    int n, W;
    scanf("%d %d", &n, &W);
    for (int i = 0; i < n; i++) scanf("%d", &w[i]);
    sort(w, w + n);

    int i = 0, j = n - 1, cars = 0;
    while (i <= j) {
        if (i < j && w[i] + w[j] <= W) {
            i++;          // 最轻的这块搭上了这趟车
        }
        j--;              // 最重的这块必然走了（不管有没有配对成功）
        cars++;
    }
    printf("%d\n", cars);
    return 0;
}
