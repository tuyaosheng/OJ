直接照着"找根 → 切中序 → 切前序 → 递归左右子树 → 拼后序"的思路写，唯一要小心的是效率：如果每次都在中序序列里从头线性扫描找根的位置，最坏情况（比如一条链）总复杂度是 $O(n^2)$，$n=10^5$ 时会超时。

解决办法：先用一个哈希表（`unordered_map<int,int>`）把"中序序列里每个值 → 它的下标"预处理出来，递归时 $O(1)$ 查到根在中序里的位置。

递归函数建议传"前序区间的起点"和"中序区间的起点、终点"（用下标表示区间，不要真的切出新数组，否则会有额外的拷贝开销）：

```cpp
void solve(int preL, int inL, int inR) {
    // 中序区间为空
    if (inL > inR) return;
    int rootVal = pre[preL];
    int rootPos = pos[rootVal];       // 哈希表查出根在中序里的下标
    int leftSize = rootPos - inL;     // 左子树节点个数
    solve(preL + 1, inL, rootPos - 1);         // 左子树
    solve(preL + 1 + leftSize, rootPos + 1, inR); // 右子树
    post.push_back(rootVal);          // 后序：左、右都处理完，最后输出根
}
```

递归深度在树严重偏斜时可能达到 $n$，注意和其他题一样可能需要考虑栈空间。
