#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++)      // 外层：第 i 行
    {
        for (int j = 1; j <= i; j++)  // 内层：这一行打 i 个星
            cout << "*";
        cout << endl;                 // 一行打完再换行
    }
    return 0;
}
