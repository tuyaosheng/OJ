#include <iostream>
#include <queue>
using namespace std;

int main()
{
    int m;
    cin >> m;
    queue<long long> q;              // FIFO 队列
    for (int i = 0; i < m; i++)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            long long x;
            cin >> x;
            q.push(x);               // 入队到队尾
        }
        else                         // op == 2
        {
            cout << q.front() << "\n";  // 取队首
            q.pop();                 // 队首出队
        }
    }
    return 0;
}
