#include <iostream>
using namespace std;

// 递归求 x 的 n 次方
long long power(int x, int n)
{
    if (n == 0) return 1;            // 边界：任何数的 0 次方是 1
    return (long long)x * power(x, n - 1);  // 递推：x^n = x * x^(n-1)
}

int main()
{
    int x, n;
    cin >> x >> n;
    cout << power(x, n) << endl;
    return 0;
}
