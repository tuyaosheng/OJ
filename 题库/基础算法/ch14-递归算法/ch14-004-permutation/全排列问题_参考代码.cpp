#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream> —— cin / cout
//
// 全排列问题：按字典序输出 1..n 的所有排列。
//
// 思路：一个位置一个位置地填。第 pos 位可以填任何"还没用过"的数字。
//   填满 n 个位置就得到一个完整排列，输出它。
//
// 这就是【回溯 Backtracking】框架，三步曲：
//   ① 尝试（做选择 + 打标记）
//   ② 递归（去处理下一个位置）
//   ③ ★撤销★（擦掉标记，好让其他分支能重新使用这个数字）
//
// 第 ③ 步是灵魂，漏掉它会导致大量排列丢失。
// 复杂度：O(n! * n)。n=8 时 8! = 40320 个排列。

const int N = 10;

int path[N];        // path[i] = 第 i 个位置填的数字
bool used[N];       // used[v] = 数字 v 是否已经被用掉了
int n;

void dfs(int pos)
{
    // 递归边界：n 个位置都填满了，输出这个完整的排列
    if (pos > n)
    {
        for (int i = 1; i <= n; i++)
            cout << path[i] << (i == n ? "\n" : " ");
        return;
    }

    // 在第 pos 个位置上，从小到大依次尝试每一个数字
    // （从小到大枚举 => 生成的排列天然就是字典序，不必额外排序）
    for (int v = 1; v <= n; v++)
    {
        if (used[v]) continue;      // v 已经用过了，排列中不能重复，跳过

        path[pos] = v;              // ① 尝试：把 v 填在第 pos 位
        used[v] = true;             //    标记 v 已被占用

        dfs(pos + 1);               // ② 递归：去填下一个位置

        used[v] = false;            // ③ ★撤销★ 关键的一行！
        // 释放 v，让后面的分支还能用它。
        // 若漏掉这行，探索完一个分支后 v 会一直被"占着"，
        // 导致其他分支误以为 v 不可用，大量排列丢失。
    }
}

int main()
{
    cin >> n;

    dfs(1);     // 从第 1 个位置开始填

    return 0;
}
