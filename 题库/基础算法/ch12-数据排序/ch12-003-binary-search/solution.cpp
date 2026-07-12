// 二分查找：先排序，再对每次询问用二分定位 x 第一次出现的位置。
// 思路：有序数组里，每次比较中间元素，就能砍掉一半搜索范围 -> 单次 O(log n)。
//       暴力扫描是 O(nq) = 100 亿，必然超时；排序+二分是 O((n+q) log n)，轻松通过。
// 本题要的是"第一次出现的位置"，即二分下界（lower_bound）。
#include <iostream>
#include <algorithm>
using namespace std;

const int N = 100005;

int a[N];
int n, q;

// 手写二分：返回 x 第一次出现的下标（0-based）；没找到返回 -1
int findFirst(int x)
{
    int l = 0, r = n - 1;
    int pos = -1;               // 记录找到的位置，初始为"没找到"

    while (l <= r)
    {
        int mid = l + (r - l) / 2;      // 这样写可以避免 (l+r) 溢出

        if (a[mid] >= x)
        {
            // a[mid] >= x，说明答案不可能在 mid 右边
            if (a[mid] == x)
                pos = mid;      // 找到一个，但左边可能还有更早的，先记下来
            r = mid - 1;        // 继续到左半边找，力求找到最靠左的那个
        }
        else
        {
            // a[mid] < x，x 只可能在右半边
            l = mid + 1;
        }
    }

    return pos;
}

int main()
{
    ios::sync_with_stdio(false);    // 10 万级读写，必须关同步
    cin.tie(nullptr);

    cin >> n >> q;

    for (int i = 0; i < n; i++)
        cin >> a[i];

    // 关键前提：二分查找要求数据有序！先排序
    sort(a, a + n);

    while (q--)
    {
        int x;
        cin >> x;

        int pos = findFirst(x);

        if (pos == -1)
            cout << -1 << "\n";
        else
            cout << pos + 1 << "\n";    // 下标 0-based 转成题目要求的 1-based 位置
    }

    return 0;
}
