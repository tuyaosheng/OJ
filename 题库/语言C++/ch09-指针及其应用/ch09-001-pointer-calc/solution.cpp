#include <iostream>
using namespace std;

int main()
{
    int a, b;
    cin >> a >> b;
    int *pa = &a, *pb = &b;          // pa 指向 a，pb 指向 b
    // *pa 就是 a 的值，*pb 就是 b 的值
    long long sum = (long long)(*pa) + (*pb);
    long long prod = (long long)(*pa) * (*pb);
    cout << sum << " " << prod << endl;
    return 0;
}
