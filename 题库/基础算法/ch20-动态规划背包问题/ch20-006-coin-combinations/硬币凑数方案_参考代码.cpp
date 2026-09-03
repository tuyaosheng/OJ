#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
typedef long long ll;
const ll MOD = 1000000007;

const int MAXS = 1005;
ll dp[MAXS];
int n, S;

int main() {
    cin >> n >> S;
    dp[0] = 1;
    for (int i = 1; i <= n; i++) {
        int w;
        cin >> w;
        for (int j = w; j <= S; j++) {
            dp[j] = (dp[j] + dp[j - w]) % MOD;
        }
    }
    cout << dp[S] << "\n";
    return 0;
}
