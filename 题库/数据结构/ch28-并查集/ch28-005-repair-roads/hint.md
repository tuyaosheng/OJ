```cpp
struct Road { int u, v, t; };
vector<Road> roads(m);
// 读入后按 t 从小到大排序
sort(roads.begin(), roads.end(), [](const Road& a, const Road& b) {
    return a.t < b.t;
});

int components = n;
int answer = -1;
for (auto& r : roads) {
    int ru = find(r.u), rv = find(r.v);
    if (ru != rv) {
        fa[ru] = rv;
        components--;
        if (components == 1) {
            answer = r.t;
            break;   // 已经全部连通，后面竣工更晚的公路不用再看
        }
    }
}
cout << answer << '\n';
```

注意 $n=1$ 时（只有一个村庄）天然"全部连通"，答案应该是 $0$（不需要修任何路）——可以在循环开始前特判 `components == 1` 的情况，或者让 `answer` 初始值按题目约定处理（本题的测试数据里 $n=1$ 时期望输出是 `0`）。

一旦找到答案立刻 `break`，不需要把剩下的公路也处理完，这是本题一个小的效率优化点。
