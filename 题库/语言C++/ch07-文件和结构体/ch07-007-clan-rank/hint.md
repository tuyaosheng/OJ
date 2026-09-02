标准的**结构体 + 多关键字排序**：

```cpp
bool cmp(const Team &a, const Team &b) {
    if (a.score != b.score) return a.score > b.score;  // 第一关键字：积分降序
    if (a.time  != b.time)  return a.time  < b.time;   // 第二关键字：用时升序
    return a.name < b.name;                            // 第三关键字：队名升序
}
```

模板套路：**逐层"不相等就分胜负，相等交给下一层"**。cmp 里只写 `>` / `<`，绝不写 `>=`（破坏严格弱序会运行错误）。
