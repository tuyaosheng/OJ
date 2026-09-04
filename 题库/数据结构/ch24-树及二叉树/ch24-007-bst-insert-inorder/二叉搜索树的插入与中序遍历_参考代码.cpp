// 本题用到 vector、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <iostream>
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    Node *left, *right;
};

Node* insert(Node* root, int v) {
    if (!root) return new Node{v, nullptr, nullptr};
    if (v < root->val) root->left = insert(root->left, v);
    else root->right = insert(root->right, v);
    return root;
}

vector<int> ans;
void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    ans.push_back(root->val);
    inorder(root->right);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    Node* root = nullptr;
    for (int i = 0; i < n; i++) {
        int v;
        cin >> v;
        root = insert(root, v);
    }
    inorder(root);
    for (size_t i = 0; i < ans.size(); i++) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}
