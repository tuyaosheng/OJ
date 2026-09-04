#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//   <queue>    —— queue

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    queue<int> q;
    while (n--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            int x;
            cin >> x;
            q.push(x);            // 入队，加在队尾
        }
        else if (op == 2)
        {
            cout << q.front() << "\n";  // pop() 不返回值，要先 front() 记下来
            q.pop();
        }
        else                      // op == 3
        {
            cout << q.size() << "\n";
        }
    }
    return 0;
}
