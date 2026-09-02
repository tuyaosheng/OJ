// 挂机金币
// 考点：while 条件循环、long long
// 每秒收益翻倍，问累计 >= k 至少需要几秒
#include <iostream>
using namespace std;

int main()
{
    long long a, k;                 // k 最大 1e17，必须 long long
    cin >> a >> k;
    long long sum = 0, gain = a;    // sum: 累计金币；gain: 这一秒的收益
    int t = 0;
    while (sum < k) {               // 攒够就停：条件循环用 while
        sum += gain;
        gain *= 2;                  // 下一秒收益翻倍
        t++;
    }
    cout << t << endl;
    return 0;
}
