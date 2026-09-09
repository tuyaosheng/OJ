```cpp
int fa[50005], rel[50005];  // rel[x]：x 相对于 fa[x] 的手势差（0/1/2），0=相同，1=x赢fa[x]，2=x输fa[x]

int find(int x) {
    if (fa[x] == x) return x;
    int root = find(fa[x]);
    rel[x] = (rel[x] + rel[fa[x]]) % 3;
    fa[x] = root;
    return root;
}

int falseCount = 0;
for (每条陈述 k u v) {
    if (k == 2 && u == v) { falseCount++; continue; }  // "自己赢自己"，直接判假
    int expected = (k == 1) ? 0 : 1;                    // 期望的手势差：相同是0，u赢v是1
    int ru = find(u), rv = find(v);
    if (ru == rv) {
        int actual = ((rel[u] - rel[v]) % 3 + 3) % 3;   // C++ 里负数取模结果可能是负的，要修正
        if (actual != expected) falseCount++;
    } else {
        fa[ru] = rv;
        rel[ru] = ((expected - rel[u] + rel[v]) % 3 + 3) % 3;
    }
}
cout << falseCount << '\n';
```

推导 `rel[ru]` 的公式时，把"手势差"想象成一个可以像距离一样"累加"的量（模 3 意义下）：想让 $u$ 相对 $v$ 的手势差最终等于 `expected`，而 $u$ 相对 $ru$ 的差是 `rel[u]`、$v$ 相对 $rv$ 的差是 `rel[v]`，中间还差一个"$ru$ 相对 $rv$ 的差"没确定——解一个模 3 的等式就能得到公式，和练3的异或公式是完全一样的推导方式，只是把"模 2 异或"换成了"模 3 加减"。

C++ 的 `%` 运算符对负数取模可能得到负数（比如 `(-1) % 3` 是 `-1` 不是 `2`），所以每次取模后都要 `+3` 再 `%3` 修正回 $[0,2]$ 的范围，这是本题最容易被忽略的坑。
