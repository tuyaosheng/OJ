用层序遍历（BFS），但这次队列里**把空指针也放进去**（不再跳过），依次弹出并检查：

- 一旦弹出了一个空节点，就把一个标记 `seenNull` 设为真。
- 之后如果再弹出一个**非空**节点，说明"空位之后又出现了节点"，直接判定不是完全二叉树。
- 队列耗尽都没出现这种情况，就是完全二叉树。

```cpp
bool isComplete(Node* root) {
    queue<Node*> q;
    q.push(root);
    bool seenNull = false;
    while (!q.empty()) {
        Node* cur = q.front(); q.pop();
        if (!cur) { seenNull = true; continue; }
        if (seenNull) return false;
        q.push(cur->left);
        q.push(cur->right);
    }
    return true;
}
```

注意这里和例2的层序遍历不同：例2是"孩子存在才入队"，这题是"孩子不管存不存在都入队"（空指针也入队），因为正是要检测空位后面还有没有节点。
