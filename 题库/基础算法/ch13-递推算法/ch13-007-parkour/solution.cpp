// 跑酷跳台阶·陷阱版
// 考点：线性递推 + 障碍状态置零 + 取模
#include <iostream>
#include <cstring>
using namespace std;

const int MOD = 1000000007;
bool trap[100005];
long long f[100005];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, k;
    cin >> n >> k;
    for (int i = 0; i < k; i++) {
        int x;
        cin >> x;
        trap[x] = true;
    }
    f[0] = 1;                          // 地面：1 种（什么都不做）
    for (int i = 1; i <= n; i++) {
        if (trap[i]) { f[i] = 0; continue; }   // 陷阱不能落脚
        f[i] = f[i - 1];                       // 从 i-1 跳 1 级
        if (i >= 2) f[i] = (f[i] + f[i - 2]) % MOD;  // 从 i-2 跳 2 级
    }
    cout << f[n] << endl;
    return 0;
}
