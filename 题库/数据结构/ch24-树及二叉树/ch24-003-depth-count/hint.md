三个量可以写成一个递归函数一次性算出来，也可以分别写三个递归函数（更清晰，推荐初学时这样写）：

```cpp
int depth(Node* r) {
    if (!r) return 0;
    return 1 + max(depth(r->left), depth(r->right));
}
int count(Node* r) {
    if (!r) return 0;
    return 1 + count(r->left) + count(r->right);
}
int leafCount(Node* r) {
    if (!r) return 0;
    if (!r->left && !r->right) return 1;
    return leafCount(r->left) + leafCount(r->right);
}
```

注意"叶子节点"的判断必须是"左右孩子都为空"，只判断其中一个孩子为空是不够的（那样只是"没有左孩子"或"没有右孩子"，不代表是叶子）。
