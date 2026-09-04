// 本题用到 algorithm（max）、climits（LLONG_MIN）、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <algorithm> <climits> <iostream>
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

long long best = LLONG_MIN;

long long gain(Node* node) {
    if (!node) return 0;
    long long l = max(0LL, gain(node->left));
    long long r = max(0LL, gain(node->right));
    best = max(best, (long long)node->val + l + r);
    return node->val + max(l, r);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    Node* root = build();
    gain(root);
    cout << best << '\n';
    return 0;
}
