**第一步：建图**。两两比较所有单词，如果恰好只有一个字母不同就连边。$N \le 2000$、单词长度 $\le 10$，两两比较的复杂度是 $O(N^2 \cdot L)$，大约 $4\times10^7$，可以接受。

```cpp
bool oneDiff(const string& a, const string& b) {
    int diff = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) diff++;
        if (diff > 1) return false;  // 提前退出，不用比较完整个单词
    }
    return diff == 1;
}

vector<int> g[2005];
for (int i = 0; i < N; i++)
    for (int j = i + 1; j < N; j++)
        if (oneDiff(words[i], words[j])) {
            g[i].push_back(j);
            g[j].push_back(i);
        }
```

**第二步：BFS 求最短路**。把 `begin` 对应的下标作为起点跑一遍 BFS，记录每个点第一次被访问时的距离（层数），`end` 对应下标的距离就是答案；如果 BFS 结束后 `end` 还没被访问过，说明不可达，输出 `-1`。

用 `map<string,int>` 把单词映射到下标，方便根据 `begin`/`end` 的字符串找到对应的图节点编号。
