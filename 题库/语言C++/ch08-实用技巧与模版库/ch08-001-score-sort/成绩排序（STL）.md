# 讲解稿：成绩排序（STL）（ch08-001）

## 一、题目解读

给出 n 个同学的姓名和分数，按"分数高的在前，分数相同名字字典序小的在前"排序输出。课件例 8.1。本章的核心是 **STL（标准模版库）**——C++ 内置的现成工具，今天先用最常用的 `sort`。

## 二、思路分析

第 5 章我们手写过冒泡排序，n=1000 还行，但这题 n 达 **10 万**，冒泡的 n²=100 亿必然超时。C++ 标准库的 `sort` 是 O(n log n) 的高效排序，直接拿来用。

排序规则两级，用比较函数 cmp 表达：
- 分数不同 → 分数大的在前（`a.score > b.score`）；
- 分数相同 → 名字字典序小的在前（`a.name < b.name`，string 直接能比字典序）。

## 三、代码讲解

```cpp
bool cmp(const Student &a, const Student &b)
{
    if (a.score != b.score) return a.score > b.score;
    return a.name < b.name;
}
...
sort(a, a + n, cmp);
```

- **`sort(首地址, 尾地址, 比较函数)`**：需要 `#include <algorithm>`。它排 `[a, a+n)` 这段（左闭右开）；
- 比较函数返回"a 是否应排在 b 前面"，多关键字用 if 链（第 7 章练过）；
- **`string` 直接用 `<` 比字典序**——这是 string 相比字符数组的巨大便利；
- **`ios::sync_with_stdio(false);`**：关闭 cin/cout 与 C 的同步，读写提速数倍，10 万数据必备的加速咒语。

## 四、易错点

1. 数据量大还用冒泡/选择——超时（TLE）。大数据一律 `sort`。
2. `sort` 的第二个参数是**尾后地址** `a+n`（不是 `a+n-1`）——左闭右开，写错会漏排最后一个或越界。
3. 忘 `#include <algorithm>`。
4. 10 万数据不加 `sync_with_stdio(false)` 可能卡在读入上。

## 五、小结

`sort` 是竞赛中使用频率最高的 STL 函数：一行完成 O(n log n) 排序，配自定义 cmp 应对任意规则。记住"左闭右开区间 `[a, a+n)`"和"string 可直接比字典序"。从此告别手写排序——把精力留给真正的算法。
