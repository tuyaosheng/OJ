#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— max

const int MAXV = 20005;
int dp[MAXV];
int V, n;

int main() {
    cin >> V;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int v;
        cin >> v;
        // 0/1 背包：把"箱子容量 V"当背包容量、"物品体积"当重量也当价值——
        // dp[j] = 用不超过 j 的容积最多能装满多少体积，一维滚动数组 j 必须从大到小
        for (int j = V; j >= v; j--) {
            dp[j] = max(dp[j], dp[j - v] + v);
        }
    }
    cout << (V - dp[V]) << "\n";   // 剩余空间 = 箱子容量 - 最多能装满的体积
    return 0;
}
