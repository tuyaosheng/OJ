// N 皇后：n×n 棋盘放 n 个皇后，任意两个不能同行、同列、同对角线。求方案数。
//
// 关键优化一：【逐行摆放】—— 规定第 i 个皇后放在第 i 行。
//   这样"不同行"自动满足，只需为每一行决定放哪一列。搜索空间大幅缩小。
//
// 关键优化二：【O(1) 判断冲突】—— 利用对角线的数学性质：
//   同一条主对角线（↘）上，row - col 是常数；
//   同一条副对角线（↙）上，row + col 是常数。
//   于是用三个标记数组即可 O(1) 判断，无需逐个检查前面的皇后。
//
// 关键优化三：【剪枝】—— 发现位置会被攻击就立刻 continue，不再往下递归。
//
// 答案参考：n=4 -> 2，n=8 -> 92，n=12 -> 14200。
#include <iostream>
using namespace std;

const int N = 15;

int n;
int ans = 0;

bool colUsed[N];        // 第 col 列是否已被占用
bool diag1[N * 2 + 5];  // 主对角线（row - col 为常数），下标 +n 防止负数
bool diag2[N * 2 + 5];  // 副对角线（row + col 为常数）

void dfs(int row)
{
    // n 行都摆好了，说明找到了一个完整的合法方案
    if (row > n)
    {
        ans++;
        return;
    }

    // 在第 row 行，尝试每一列
    for (int col = 1; col <= n; col++)
    {
        // ★ 剪枝：这个位置会被已放置的皇后攻击，直接跳过，不浪费时间往下递归
        if (colUsed[col] || diag1[row - col + n] || diag2[row + col])
            continue;

        // ① 做选择：在 (row, col) 放一个皇后，占住这一列和两条对角线
        colUsed[col] = true;
        diag1[row - col + n] = true;
        diag2[row + col] = true;

        // ② 递归：去摆下一行
        dfs(row + 1);

        // ③ ★撤销★：把皇后拿走，释放这一列和两条对角线
        // 因为标记的含义是"在我当前正在摆的这个方案里被占用"，
        // 拿走皇后后自然要释放，好让这一行去尝试别的列。
        colUsed[col] = false;
        diag1[row - col + n] = false;
        diag2[row + col] = false;
    }
}

int main()
{
    cin >> n;

    dfs(1);         // 从第 1 行开始摆

    cout << ans << "\n";

    return 0;
}
