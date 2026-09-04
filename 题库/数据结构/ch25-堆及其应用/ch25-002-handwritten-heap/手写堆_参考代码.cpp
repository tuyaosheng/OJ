// 本题用到 algorithm（swap）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <algorithm> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 300005;
int heap_[MAXN], sz = 0;

void push(int x) {
    heap_[++sz] = x;
    int i = sz;
    while (i > 1 && heap_[i / 2] > heap_[i]) {
        swap(heap_[i / 2], heap_[i]);
        i /= 2;
    }
}

int popMin() {
    int ret = heap_[1];
    heap_[1] = heap_[sz--];
    int i = 1;
    while (true) {
        int smallest = i, l = 2 * i, r = 2 * i + 1;
        if (l <= sz && heap_[l] < heap_[smallest]) smallest = l;
        if (r <= sz && heap_[r] < heap_[smallest]) smallest = r;
        if (smallest == i) break;
        swap(heap_[i], heap_[smallest]);
        i = smallest;
    }
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            int x;
            cin >> x;
            push(x);
        } else {
            if (sz == 0) {
                cout << -1 << '\n';
            } else {
                cout << popMin() << '\n';
            }
        }
    }
    return 0;
}
