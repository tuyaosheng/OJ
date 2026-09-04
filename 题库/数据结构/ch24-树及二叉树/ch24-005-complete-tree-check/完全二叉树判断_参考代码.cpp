// 本题用到 queue、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <queue> <iostream>
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

bool isComplete(Node* root) {
    queue<Node*> q;
    q.push(root);
    bool seenNull = false;
    while (!q.empty()) {
        Node* cur = q.front(); q.pop();
        if (!cur) { seenNull = true; continue; }
        if (seenNull) return false;
        q.push(cur->left);
        q.push(cur->right);
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    Node* root = build();
    cout << (isComplete(root) ? "Yes" : "No") << '\n';
    return 0;
}
