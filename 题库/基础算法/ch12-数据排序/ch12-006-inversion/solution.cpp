// 逆序对：求满足 i < j 且 a[i] > a[j] 的下标对个数。
//
// 核心：在【归并排序的合并步骤】里顺便数逆序对。
//   合并时左半 L、右半 R 都已有序。若 L[i] > R[j]，说明 L 中从 i 到 mid 的所有数
//   都 > R[j]（因为 L 有序），而它们的下标都比 j 小 —— 一口气产生 (mid - i + 1) 个逆序对。
//
// 复杂度：O(n log n)。暴力两重循环是 O(n^2) = 100 亿，必然超时。
#include <iostream>
using namespace std;

const int N = 100005;

int a[N];
int tmp[N];         // 合并时的临时数组
long long ans = 0;  // 逆序对最多约 n^2/2 = 50 亿，必须用 long long！

// 归并排序 [l, r]（闭区间），同时累加逆序对
void mergeSort(int l, int r)
{
    if (l >= r) return;             // 只剩 0 或 1 个数，无需排序，也没有逆序对

    int mid = l + (r - l) / 2;

    mergeSort(l, mid);              // 先排好左半边
    mergeSort(mid + 1, r);          // 再排好右半边

    // 合并两个有序的半边，并统计"跨越左右"的逆序对
    int i = l;          // 左半边的指针
    int j = mid + 1;    // 右半边的指针
    int k = l;          // tmp 数组的写入位置

    while (i <= mid && j <= r)
    {
        if (a[i] <= a[j])
        {
            // 左边的数更小（或相等），取左边。不产生逆序对。
            // 注意必须是 <= ：相等【不算】逆序对（定义是严格大于）。
            // 若写成 <，相等的情况会跑到 else 分支，把相等也算进去，答案偏大！
            tmp[k++] = a[i++];
        }
        else
        {
            // a[i] > a[j]：取右边的数。
            // 关键！左半边有序，a[i] > a[j] 意味着 a[i], a[i+1], ..., a[mid] 全都 > a[j]，
            // 而它们的下标都小于 j 的下标 —— 于是一次性产生 (mid - i + 1) 个逆序对。
            tmp[k++] = a[j++];
            ans += mid - i + 1;
        }
    }

    // 把某一边剩下的元素直接搬过来（剩下的不再产生新的逆序对）
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r)   tmp[k++] = a[j++];

    // 把合并好的有序段写回原数组
    for (int p = l; p <= r; p++)
        a[p] = tmp[p];
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 0; i < n; i++)
        cin >> a[i];

    mergeSort(0, n - 1);

    cout << ans << "\n";

    return 0;
}
