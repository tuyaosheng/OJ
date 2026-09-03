#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int a, b, c;
    cin >> a >> b >> c;
    int maxn = a;            // 先让 a 站上擂台
    if (b > maxn) maxn = b;  // b 挑战：更大就取代
    if (c > maxn) maxn = c;  // c 挑战：更大就取代
    cout << maxn << "\n";
    return 0;
}
