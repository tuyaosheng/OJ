`string` 的两个成员函数正好配套：

- `s.find(w, pos)`：从下标 pos 开始找 w，找到返回起始下标，找不到返回 `string::npos`；
- `s.replace(p, len, string(len, '*'))`：把从 p 开始的 len 个字符换成 len 个 `*`。

循环骨架：

```cpp
size_t p = s.find(w);
while (p != string::npos) {
    s.replace(p, w.size(), string(w.size(), '*'));
    p = s.find(w, p + w.size());   // 从替换段之后继续
}
```

注意 `aaa` 中过滤 `aa`：第一处替换后变成 `**a`，从下标 2 继续找——剩下的 `a` 不构成敏感词，结果是 `**a`。
