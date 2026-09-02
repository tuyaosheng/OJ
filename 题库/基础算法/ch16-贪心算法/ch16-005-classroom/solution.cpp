// 教室排课
// 考点：区间贪心（活动选择问题）
// 结论：按结束时间从早到晚排序，能选就选
#include <cstdio>
#include <algorithm>
using namespace std;

struct Act { long long s, e; };
Act act[100005];

bool cmp(const Act &a, const Act &b) { return a.e < b.e; }

int main()
{
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%lld %lld", &act[i].s, &act[i].e);
    sort(act, act + n, cmp);            // 按结束时间从早到晚排序

    int cnt = 0;
    long long last_end = -1;            // 上一个被选中的班级的结束时间
    for (int i = 0; i < n; i++) {
        if (act[i].s >= last_end) {     // 和上一个不冲突（背靠背也算不冲突）
            cnt++;
            last_end = act[i].e;
        }
    }
    printf("%d\n", cnt);
    return 0;
}
