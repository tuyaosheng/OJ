用一个全局下标 `pos` 表示当前读到第几个 token，写一个递归函数：

```cpp
struct Node {
    bool isNum;
    long long val;   // isNum == true 时有效
    char op;          // isNum == false 时有效，'+' 或 '*'
    Node *left, *right;
};

Node* build() {
    string tok = tokens[pos++];
    if (tok == "+" || tok == "*") {
        Node* node = new Node{false, 0, tok[0], nullptr, nullptr};
        node->left = build();   // 先递归读出左操作数（可能本身还是一棵子树）
        node->right = build();  // 再递归读出右操作数
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
```

这和"由前序+中序重建二叉树"那道题的建树思路是一脉相承的——运算符（相当于"根"）永远出现在它两个操作数之前，所以扫描到运算符就知道"这里要建一个内部节点，它的左右子树紧接着从后面的 token 里递归读出"。
