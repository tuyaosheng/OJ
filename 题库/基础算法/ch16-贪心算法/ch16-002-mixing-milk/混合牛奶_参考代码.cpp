#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— sort、min
// 考点：性价比排序贪心
// 结论：从最便宜的供应商开始买，能买多少买多少

struct Supplier { long long price, amount; };
Supplier sup[5005];

bool cmp(const Supplier &a, const Supplier &b) { return a.price < b.price; }

int main() {
    long long need, remain;
    int m;
    cin >> need >> m;
    remain = need;
    for (int i = 0; i < m; i++) cin >> sup[i].price >> sup[i].amount;
    sort(sup, sup + m, cmp);        // 单价从小到大排序

    long long cost = 0;
    for (int i = 0; i < m && remain > 0; i++) {
        long long buy = min(sup[i].amount, remain);  // 这家最多买这么多（不能超过还需要的量）
        cost += buy * sup[i].price;
        remain -= buy;
    }
    cout << cost << "\n";
    return 0;
}
