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
        int w, p;
        cin >> w >> p;
        for (int j = W; j >= w; j--) {
            dp[j] = max(dp[j], dp[j - w] + p);
        }
    }
    cout << dp[W] << "\n";
    return 0;
}
