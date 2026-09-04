// 本题用到 queue、vector、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <queue> <vector> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;
int a[MAXN];
int n, pos_ = 0;

struct Node {
    int val;
    Node *left, *right;
};

Node* build() {
    int v = a[pos_++];
    if (v == -1) return nullptr;
    Node* root = new Node{v, nullptr, nullptr};
    root->left = build();
    root->right = build();
    return root;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    Node* root = build();

    vector<int> ans;
    queue<Node*> q;
    if (root) q.push(root);
    while (!q.empty()) {
        Node* cur = q.front(); q.pop();
        ans.push_back(cur->val);
        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
    for (size_t i = 0; i < ans.size(); i++) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}
