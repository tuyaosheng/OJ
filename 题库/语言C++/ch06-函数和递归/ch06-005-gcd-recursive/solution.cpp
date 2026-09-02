#include <iostream>
using namespace std;

// 递归辗转相除：gcd(m,n) = gcd(n, m%n)，n 为 0 时返回 m
int gcd(int m, int n)
{
    if (n == 0) return m;        // 边界：余数为 0，m 即答案
    return gcd(n, m % n);        // 递推：问题缩小为 gcd(n, m%n)
}

int main()
{
    int m, n;
    cin >> m >> n;
    cout << gcd(m, n) << endl;
    return 0;
}
