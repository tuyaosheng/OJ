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
        for (int j = V; j >= v; j--) {
            dp[j] = max(dp[j], dp[j - v] + v);
        }
    }
    cout << (V - dp[V]) << "\n";
    return 0;
}
