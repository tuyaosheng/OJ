// 本题只用到 vector（存节点）、以及 cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <iostream>
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

vector<int> in_order, post_order;

void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    in_order.push_back(root->val);
    inorder(root->right);
}

void postorder(Node* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    post_order.push_back(root->val);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    Node* root = build();
    inorder(root);
    postorder(root);
    for (size_t i = 0; i < in_order.size(); i++) {
        if (i) cout << ' ';
        cout << in_order[i];
    }
    cout << '\n';
    for (size_t i = 0; i < post_order.size(); i++) {
        if (i) cout << ' ';
        cout << post_order[i];
    }
    cout << '\n';
    return 0;
}
