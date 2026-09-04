// 本题用到 queue（priority_queue）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <queue> <iostream>
#include <bits/stdc++.h>
using namespace std;

priority_queue<int> lower_;
priority_queue<int, vector<int>, greater<int>> upper_;

void insert(int x) {
    if (lower_.empty() || x <= lower_.top()) lower_.push(x);
    else upper_.push(x);

    if (lower_.size() > upper_.size() + 1) {
        upper_.push(lower_.top());
        lower_.pop();
    } else if (upper_.size() > lower_.size()) {
        lower_.push(upper_.top());
        upper_.pop();
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        insert(x);
        cout << lower_.top() << '\n';
    }
    return 0;
}
