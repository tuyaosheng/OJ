#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— max

const int MAXW = 1005;
int dp[MAXW];
int W, n;

int main() {
    cin >> W >> n;
    for (int i = 1; i <= n; i++) {
        int w, p;      // w=这件装备的负重，p=这件装备的战力加成
        cin >> w >> p;
        // 标准 0/1 背包：每件装备只能背一次，一维滚动数组 j 必须从大到小遍历
        for (int j = W; j >= w; j--) {
            dp[j] = max(dp[j], dp[j - w] + p);
        }
    }
    cout << dp[W] << "\n";
    return 0;
}
