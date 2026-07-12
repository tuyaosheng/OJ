#include <iostream>
using namespace std;

// 辗转相除法求最大公约数
int gcd(int a, int b)
{
    while (b != 0)
    {
        int r = a % b;   // 余数
        a = b;
        b = r;
    }
    return a;            // b 为 0 时，a 就是最大公约数
}

int main()
{
    int x, y;
    cin >> x >> y;
    // LCM = x*y/gcd，先除后乘防溢出：x/gcd 一定整除
    long long lcm = (long long)(x / gcd(x, y)) * y;
    cout << lcm << endl;
    return 0;
}
