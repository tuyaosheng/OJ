// 本题用到 vector、string、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <string> <sstream> <iostream>
#include <bits/stdc++.h>
using namespace std;

struct Node {
    bool isNum;
    long long val;
    char op;
    Node *left, *right;
};

vector<string> tokens;
int pos_ = 0;

Node* build() {
    string tok = tokens[pos_++];
    if (tok == "+" || tok == "*") {
        Node* node = new Node{false, 0, tok[0], nullptr, nullptr};
        node->left = build();
        node->right = build();
        return node;
    } else {
        return new Node{true, stoll(tok), 0, nullptr, nullptr};
    }
}

long long eval(Node* root) {
    if (root->isNum) return root->val;
    long long l = eval(root->left), r = eval(root->right);
    return root->op == '+' ? l + r : l * r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    getline(cin, line);
    stringstream ss(line);
    string tok;
    while (ss >> tok) tokens.push_back(tok);
    Node* root = build();
    cout << eval(root) << '\n';
    return 0;
}
