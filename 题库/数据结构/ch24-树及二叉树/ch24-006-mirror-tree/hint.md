递归地"自底向上"交换每个节点的左右孩子：

```cpp
void mirror(Node* root) {
    if (!root) return;
    swap(root->left, root->right);
    mirror(root->left);
    mirror(root->right);
}
```

注意 `swap` 之后再递归——递归调用的 `root->left`、`root->right` 已经是交换后的指针，但因为交换和递归本身互不影响正确性（交换的是指针，递归只是继续往下处理这两棵子树，不管它们现在叫 `left` 还是 `right`），所以谁先谁后都可以，习惯上先交换。

翻转之后，用例2学过的层序遍历模板输出结果即可。
