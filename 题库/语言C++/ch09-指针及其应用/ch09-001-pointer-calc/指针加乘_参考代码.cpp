#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int a, b;
    cin >> a >> b;
    int *pa = &a, *pb = &b;          // pa 指向 a，pb 指向 b
    // *pa 就是 a 的值，*pb 就是 b 的值
    long long sum = (long long)(*pa) + (*pb);
    long long prod = (long long)(*pa) * (*pb);
    cout << sum << " " << prod << "\n";
    return 0;
}
