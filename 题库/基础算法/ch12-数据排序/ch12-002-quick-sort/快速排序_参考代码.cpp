#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— swap
//
// 快速排序：分治思想的代表作。
// 思路：选一个基准数 -> 把比它小的甩到左边、比它大的甩到右边（分区）
//       -> 基准归位后，对左右两半递归做同样的事。
// 复杂度：平均 O(n log n)。n = 10 万时约 170 万次操作，很快。
//
// 注：竞赛中一般直接用 <algorithm> 里的 sort（底层就是优化版快排），
//     这里手写一遍是为了理解 O(n log n) 排序的原理。

const int N = 100005;

int a[N];

// 对区间 [l, r] 做快速排序（闭区间，两端都含）
void quickSort(int l, int r)
{
    if (l >= r) return;             // 区间里只剩 0 或 1 个数，本来就有序

    int pivot = a[(l + r) / 2];     // 取中间位置的数作为基准
    // 取中间而不是取第一个，是为了避免"数据已经有序"时退化成 O(n^2)

    int i = l, j = r;

    // 分区：让 [l..j] 里都是 <= pivot 的，[i..r] 里都是 >= pivot 的
    while (i <= j)
    {
        while (a[i] < pivot) i++;   // 从左往右，找一个不该待在左边的（>= pivot）
        while (a[j] > pivot) j--;   // 从右往左，找一个不该待在右边的（<= pivot）

        if (i <= j)
        {
            swap(a[i], a[j]);       // 交换这两个"站错队"的数
            i++;
            j--;
        }
    }

    // 此时基准已经归位，左右两半各自递归处理
    quickSort(l, j);                // 左半边
    quickSort(i, r);                // 右半边
}

int main()
{
    ios::sync_with_stdio(false);    // 关闭同步，10 万级读写必备，能快好几倍
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 0; i < n; i++)
        cin >> a[i];

    quickSort(0, n - 1);            // 排 [0, n-1] 这个闭区间

    // 输出：相邻数之间一个空格，行末不留多余空格
    for (int i = 0; i < n; i++)
        cout << a[i] << (i == n - 1 ? "\n" : " ");

    return 0;
}
