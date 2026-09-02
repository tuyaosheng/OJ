链表节点 = 数据 + 指向下一节点的指针：

```cpp
struct Node {
    int val;
    Node *next;
};
```

**头插法**三步（顺序不能乱）：

```cpp
Node *p = new Node;   // 1. 造新节点
p->val = x;
p->next = head;       // 2. 新节点指向原来的头
head = p;             // 3. 头指针改指新节点
```

遍历：`for (Node *p = head; p != nullptr; p = p->next)`。

本题用数组倒序输出当然也能过——但请按提示用链表写一遍：第三部分数据结构的链式栈、树，都从这三行头插开始。
