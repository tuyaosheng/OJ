#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    int n, x;
    cin >> n;
    cin >> x;                    // 先读第一个数
    int minn = x, maxn = x;      // 用第一个数初始化最小/最大值
    long long sum = x;
    for (int i = 2; i <= n; i++) // 读剩下的 n-1 个数
    {
        cin >> x;
        if (x < minn) minn = x;  // 打擂台更新最小值
        if (x > maxn) maxn = x;  // 更新最大值
        sum += x;
    }
    double avg = (double)sum / n;    // 强制转 double，避免整数除法
    cout << minn << " " << maxn << " "
         << fixed << setprecision(3) << avg << endl;
    return 0;
}
