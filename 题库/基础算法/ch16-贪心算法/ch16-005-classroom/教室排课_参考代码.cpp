#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— sort
// 考点：区间贪心（活动选择问题）
// 结论：按结束时间从早到晚排序，能选就选

struct Act { long long s, e; };
Act act[100005];

bool cmp(const Act &a, const Act &b) { return a.e < b.e; }

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) cin >> act[i].s >> act[i].e;
    sort(act, act + n, cmp);            // 按结束时间从早到晚排序

    int cnt = 0;
    long long last_end = -1;            // 上一个被选中的班级的结束时间
    for (int i = 0; i < n; i++) {
        if (act[i].s >= last_end) {     // 和上一个不冲突（背靠背也算不冲突）
            cnt++;
            last_end = act[i].e;
        }
    }
    cout << cnt << "\n";
    return 0;
}
