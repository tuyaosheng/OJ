// 合并果子
// 考点：贪心 + 优先队列（小根堆）
// 结论：每次合并当前最小的两堆，代价最低
#include <cstdio>
#include <queue>
#include <vector>
using namespace std;

int main()
{
    int n;
    scanf("%d", &n);
    priority_queue<long long, vector<long long>, greater<long long>> pq;  // 小根堆
    for (int i = 0; i < n; i++) {
        long long x;
        scanf("%lld", &x);
        pq.push(x);
    }

    long long total = 0;
    while (pq.size() > 1) {
        long long x = pq.top(); pq.pop();   // 最小的一堆
        long long y = pq.top(); pq.pop();   // 第二小的一堆
        total += x + y;                     // 本次合并耗费
        pq.push(x + y);                     // 合并后的新堆放回去
    }
    printf("%lld\n", total);
    return 0;
}
