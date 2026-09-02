#include <iostream>
using namespace std;

int main()
{
    int n, a[105];
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];   // 下标从 1 开始用，好数
    int temp = a[1];                  // ① 第一个元素先存起来
    for (int i = 1; i < n; i++)       // ② 从前往后，后一个往前挪
        a[i] = a[i + 1];
    a[n] = temp;                      // ③ 原第一个放到末尾
    for (int i = 1; i <= n; i++)
    {
        if (i > 1) cout << " ";       // 数与数之间输出空格（行尾无空格）
        cout << a[i];
    }
    cout << endl;
    return 0;
}
