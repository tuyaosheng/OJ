#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
typedef long long ll;

const int MAXN = 100005;
int a[MAXN], tmp[MAXN];
ll invCount = 0;

// 归并排序的“合”：把 [l, mid] 与 [mid+1, r] 两个有序区间合并成 [l, r] 的有序区间，
// 顺便统计逆序对。
void merge(int l, int mid, int r) {
    int i = l, j = mid + 1, k = l;
    while (i <= mid && j <= r) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            invCount += (mid - i + 1);  // a[i..mid] 都比 a[j] 大，一次性统计
            tmp[k++] = a[j++];
        }
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (int t = l; t <= r; t++) a[t] = tmp[t];
}

// 分治主体：分（取中点）+ 治（递归左右两半）+ 合（merge）
void mergeSort(int l, int r) {
    if (l >= r) return;          // 递归终止条件：区间只剩一个数
    int mid = (l + r) / 2;
    mergeSort(l, mid);           // 子问题：排好左半
    mergeSort(mid + 1, r);       // 子问题：排好右半
    merge(l, mid, r);            // 合并两个有序区间
}

int main() {
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];
    mergeSort(1, n);
    cout << invCount << "\n";
    return 0;
}
