// 本题用到 algorithm（swap）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <algorithm> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 300005;
int a[MAXN], n;

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    for (int i = n / 2; i >= 1; i--) siftDown(i, n);

    for (int sz = n; sz > 1; sz--) {
        swap(a[1], a[sz]);
        siftDown(1, sz - 1);
    }

    for (int i = 1; i <= n; i++) {
        if (i > 1) cout << ' ';
        cout << a[i];
    }
    cout << '\n';
    return 0;
}
