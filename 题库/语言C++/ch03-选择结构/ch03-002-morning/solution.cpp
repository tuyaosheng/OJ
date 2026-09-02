#include <iostream>
using namespace std;

int main()
{
    int t;
    cin >> t;
    if (t >= 25 && t <= 30)      // 区间判断：两个条件用 && (并且) 连接
        cout << "ok" << endl;
    else
        cout << "no" << endl;
    return 0;
}
