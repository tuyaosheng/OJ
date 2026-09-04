```cpp
queue<Node*> q;
if (root) q.push(root);
while (!q.empty()) {
    Node* cur = q.front(); q.pop();
    cout << cur->val << ' ';
    if (cur->left) q.push(cur->left);
    if (cur->right) q.push(cur->right);
}
```

注意和先序遍历的区别：先序遍历是"访问根 → 递归左子树 → 递归右子树"（用的是函数调用栈，天然后进先出）；层序遍历是"取队首、访问、把它的孩子放进队尾"（用显式队列，先进先出），两者数据结构不同，绝不能混用递归来写层序遍历。
