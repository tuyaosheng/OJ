#include <iostream>
using namespace std;

int main()
{
    int a, b, c;
    cin >> a >> b >> c;
    int maxn = a;            // 先让 a 站上擂台
    if (b > maxn) maxn = b;  // b 挑战：更大就取代
    if (c > maxn) maxn = c;  // c 挑战：更大就取代
    cout << maxn << endl;
    return 0;
}
