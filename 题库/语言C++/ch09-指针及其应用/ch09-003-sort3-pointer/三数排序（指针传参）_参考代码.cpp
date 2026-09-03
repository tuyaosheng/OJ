#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout

// 通过指针交换两个变量的值（修改的是主函数里的变量）
void mySwap(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

// 排序，使 *a <= *b <= *c
void sort3(int *a, int *b, int *c)
{
    if (*a > *b) mySwap(a, b);
    if (*a > *c) mySwap(a, c);       // 此时 *a 最小
    if (*b > *c) mySwap(b, c);       // *b <= *c
}

int main()
{
    int a, b, c;
    cin >> a >> b >> c;
    sort3(&a, &b, &c);               // 传地址，函数才能改动 a,b,c
    cout << a << " " << b << " " << c << "\n";
    return 0;
}
