#include <iostream>
using namespace std;

// 求 x 的阶乘 x! = 1*2*...*x，返回 long long 防溢出
long long fact(int x)
{
    long long s = 1;
    for (int i = 2; i <= x; i++) s *= i;
    return s;                    // 0! 和 1! 都返回 1
}

int main()
{
    int m, n;
    cin >> m >> n;
    // C(m,n) = m! / (n! * (m-n)!)，fact 函数复用三次
    long long ans = fact(m) / (fact(n) * fact(m - n));
    cout << ans << endl;
    return 0;
}
