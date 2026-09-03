#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

int main()
{
    int a, b, t;         // t 是"空瓶子"：临时变量
    cin >> a >> b;
    t = a;               // ① 把 a 的值先倒进空瓶 t
    a = b;               // ② 把 b 的值倒进 a
    b = t;               // ③ 把 t 里存的原 a 值倒进 b，交换完成
    cout << a << " " << b << "\n";
    return 0;
}
