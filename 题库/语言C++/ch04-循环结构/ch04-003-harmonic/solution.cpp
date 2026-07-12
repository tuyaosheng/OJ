#include <iostream>
using namespace std;

int main()
{
    int k;
    cin >> k;
    double s = 0;            // 部分和，是小数
    long long n = 0;
    while (s < k)            // 还没达到 k 就继续加
    {
        n++;
        s += 1.0 / n;        // 1.0/n 才是浮点除法；写 1/n 恒为 0
    }
    cout << n << endl;
    return 0;
}
