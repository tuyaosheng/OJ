#include <iostream>
using namespace std;

int main()
{
    long long sum = 0;
    int x;
    while (cin >> x)     // 一直读到文件末尾（EOF）；读取失败则退出循环
        sum += x;
    cout << sum << endl;
    return 0;
}
