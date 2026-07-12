#include <iostream>
using namespace std;

long long a[35][35];         // a[i][j]: 第 i 行第 j 个数

int main()
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        a[i][1] = a[i][i] = 1;               // 每行两端是 1
        for (int j = 2; j < i; j++)          // 中间的数由上一行递推
            a[i][j] = a[i-1][j-1] + a[i-1][j];   // 肩上两数之和
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            if (j > 1) cout << " ";
            cout << a[i][j];
        }
        cout << endl;
    }
    return 0;
}
