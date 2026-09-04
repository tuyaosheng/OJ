插入函数直接照着规则递归写：

```cpp
Node* insert(Node* root, int v) {
    if (!root) return new Node{v, nullptr, nullptr};
    if (v < root->val) root->left = insert(root->left, v);
    else root->right = insert(root->right, v);
    return root;
}
```

主函数里对每个待插入的数调用 `root = insert(root, v)`，插入完成后做一次标准的中序遍历即可。

注意：如果输入数据本身是有序的（比如从小到大或从大到小），插入出来的树会退化成一条链，这是二叉搜索树在最坏情况下的样子——这也是为什么在数据规模较大、且插入顺序不可控时，需要更高级的"自平衡二叉搜索树"（比如红黑树、Treap），不过那是超出本章范围的内容，这里 $n \le 5000$，普通递归插入完全够用。
