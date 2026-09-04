```cpp
int heap[300005], sz = 0;  // 下标从 1 开始用

void push(int x) {
    heap[++sz] = x;
    int i = sz;
    while (i > 1 && heap[i / 2] > heap[i]) {
        swap(heap[i / 2], heap[i]);
        i /= 2;
    }
}

int popMin() {
    int ret = heap[1];
    heap[1] = heap[sz--];
    int i = 1;
    while (true) {
        int smallest = i, l = 2 * i, r = 2 * i + 1;
        if (l <= sz && heap[l] < heap[smallest]) smallest = l;
        if (r <= sz && heap[r] < heap[smallest]) smallest = r;
        if (smallest == i) break;
        swap(heap[i], heap[smallest]);
        i = smallest;
    }
    return ret;
}
```

下沉时**一定要比较左右两个孩子，选较小的那个去交换**——只跟其中一个孩子比较是常见的错误，会导致堆序性质被破坏（想一想：如果只跟左孩子比较、忽略了更小的右孩子，交换后右孩子反而比新的父节点还小，堆的性质就不成立了）。
