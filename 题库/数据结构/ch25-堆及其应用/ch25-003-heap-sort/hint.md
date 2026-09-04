建堆有两种方式：

1. **逐个插入**：把 $n$ 个数一个个 `push` 进堆，每次 $O(\log n)$，总共 $O(n \log n)$。
2. **从下往上批量建堆**（更快，$O(n)$）：把数组直接当成一棵完全二叉树，从**最后一个非叶子节点**（下标 $n/2$）开始，倒着往前对每个节点做一次"下沉"，到根节点为止。

```cpp
int a[300005], n;

void siftDown(int i, int sz) {
    while (true) {
        int biggest = i, l = 2 * i, r = 2 * i + 1;
        if (l <= sz && a[l] > a[biggest]) biggest = l;
        if (r <= sz && a[r] > a[biggest]) biggest = r;
        if (biggest == i) break;
        swap(a[i], a[biggest]);
        i = biggest;
    }
}

// 建堆（大根堆）
for (int i = n / 2; i >= 1; i--) siftDown(i, n);

// 排序：不断把堆顶换到末尾，缩小堆范围，重新下沉
for (int sz = n; sz > 1; sz--) {
    swap(a[1], a[sz]);
    siftDown(1, sz - 1);
}
```

这里用的是下标从 $1$ 开始存数据的约定（`a[0]` 空着不用），和上一题手写堆保持一致。排序结束后 `a[1..n]` 就是从小到大的结果。
