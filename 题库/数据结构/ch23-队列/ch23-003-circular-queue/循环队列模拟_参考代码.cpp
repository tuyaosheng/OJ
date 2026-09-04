#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <vector>   —— vector

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<int> a(m);
    int front = 0, rear = 0;      // front 指向队首，rear 指向下一个入队的位置
    while (n--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            int x;
            cin >> x;
            a[rear] = x;
            rear = (rear + 1) % m;   // 取模：走到末尾自动绕回开头
        }
        else                      // op == 2
        {
            cout << a[front] << "\n";
            front = (front + 1) % m;
        }
    }
    return 0;
}
