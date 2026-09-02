#include <iostream>
using namespace std;

int a[100005];
long long s[100005];

int main()
{
    ios::sync_with_stdio(false);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        s[i] = s[i - 1] + a[i];      // 前缀和递推
    }
    while (q--)
    {
        int l, r;
        cin >> l >> r;
        cout << s[r] - s[l - 1] << "\n";  // 区间和 = s[r]-s[l-1]
    }
    return 0;
}
