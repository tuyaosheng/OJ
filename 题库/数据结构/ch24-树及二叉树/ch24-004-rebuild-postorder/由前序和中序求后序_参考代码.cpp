// 本题用到 unordered_map、vector、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <unordered_map> <vector> <iostream>
#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> pre_, in_, post_;
unordered_map<int, int> pos_of;

void solve(int preL, int inL, int inR) {
    if (inL > inR) return;
    int rootVal = pre_[preL];
    int rootPos = pos_of[rootVal];
    int leftSize = rootPos - inL;
    solve(preL + 1, inL, rootPos - 1);
    solve(preL + 1 + leftSize, rootPos + 1, inR);
    post_.push_back(rootVal);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    pre_.resize(n);
    in_.resize(n);
    for (int i = 0; i < n; i++) cin >> pre_[i];
    for (int i = 0; i < n; i++) {
        cin >> in_[i];
        pos_of[in_[i]] = i;
    }
    post_.reserve(n);
    solve(0, 0, n - 1);
    for (int i = 0; i < n; i++) {
        if (i) cout << ' ';
        cout << post_[i];
    }
    cout << '\n';
    return 0;
}
