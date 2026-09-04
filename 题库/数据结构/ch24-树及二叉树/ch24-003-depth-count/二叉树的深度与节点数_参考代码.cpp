// 本题用到 algorithm（max）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <algorithm> <iostream>
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

int depth(Node* r) {
    if (!r) return 0;
    return 1 + max(depth(r->left), depth(r->right));
}

int countNodes(Node* r) {
    if (!r) return 0;
    return 1 + countNodes(r->left) + countNodes(r->right);
}

int leafCount(Node* r) {
    if (!r) return 0;
    if (!r->left && !r->right) return 1;
    return leafCount(r->left) + leafCount(r->right);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    Node* root = build();
    cout << depth(root) << ' ' << countNodes(root) << ' ' << leafCount(root) << '\n';
    return 0;
}
