// 选择排序：每一轮从"还没排好的部分"里挑出最小的数，放到当前位置。
// 思路：第 i 轮确定第 i 个位置该放谁 —— 到 [i, n) 里找最小值，和 a[i] 交换。
// 复杂度：两重循环 O(n^2)。n = 2000 时约 200 万次比较，很快。
#include <iostream>
#include <algorithm>    // swap
using namespace std;

const int N = 2005;

int a[N];

int main()
{
    ios::sync_with_stdio(false);

    int n;
    cin >> n;

    for (int i = 0; i < n; i++)
        cin >> a[i];

    // 选择排序主体
    // 外层只需到 n-2：前 n-1 个位置都确定后，最后一个位置自然是剩下的最大数
    for (int i = 0; i < n - 1; i++)
    {
        int minIdx = i;                 // 先假设 i 位置上的就是最小的

        // 到后面（还没排好的部分）找找有没有更小的
        for (int j = i + 1; j < n; j++)
            if (a[j] < a[minIdx])
                minIdx = j;             // 记住更小者的【下标】，而不是值

        // 把找到的最小数换到 i 位置上，这个位置就永远确定了
        swap(a[i], a[minIdx]);
    }

    // 输出：相邻数之间一个空格，行末不留多余空格
    for (int i = 0; i < n; i++)
        cout << a[i] << (i == n - 1 ? "\n" : " ");

    return 0;
}
