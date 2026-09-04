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
    int front = 0, rear = 0, cnt = 0;   // cnt 单独记录元素个数，避免只靠下标分不清空/满
    while (n--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            int x;
            cin >> x;
            if (cnt == m)
            {
                cout << "Full\n";
            }
            else
            {
                a[rear] = x;
                rear = (rear + 1) % m;
                cnt++;
            }
        }
        else if (op == 2)
        {
            if (cnt == 0)
            {
                cout << "Empty\n";
            }
            else
            {
                cout << a[front] << "\n";
                front = (front + 1) % m;
                cnt--;
            }
        }
        else                      // op == 3
        {
            cout << cnt << "\n";
        }
    }
    return 0;
}
